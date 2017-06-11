/*!
    \file
    \brief flipflip's Arduino Uno stuff: Adeunis ARF32 bluetooth module (see \ref PROJECTS_FFPHONE_ARF32)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE_ARF32 ARF32
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: interrupt things

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "arf32.h"
#include "lmx9830.h"

/* ***** low-level serial stuff ********************************************** */

// TX:  ARF32 --> Arduino
// RX:  ARF32 <-- Arduino
// RTS: ARF32 --> Arduino  "request to send" (can I send data?)
// CTS: ARF32 <-- Arduino  "clear to send" (yes, you can send)

static volatile char      svTxBuf[100];    // output buffer
static volatile uint8_t   svTxBufHead;     // write-to-buffer pointer (index)
static volatile uint8_t   svTxBufTail;     // read-from-buffer pointer (index)
static volatile uint8_t   svTxBufSize;     // size of buffered data
static volatile uint8_t   svTxBufPeak;     // peak output buffer size

static volatile char      svRxBuf[100];    // data rx buffer
static volatile uint8_t   svRxBufHead;     // write-to-buffer pointer (index)
static volatile uint8_t   svRxBufTail;     // read-from-buffer pointer (index)
static volatile uint8_t   svRxBufSize;     // size of buffered data
static volatile uint8_t   svRxBufPeak;     // peak input buffer size
static volatile uint16_t  svRxBufDrop;     // number of dropped bytes

static OS_SEMAPHORE_t sRxReadySem;


// calculate baudrate parameters
#define BAUD 9600
//#define BAUD_TOL 5 // STFU, utils/setbaud.h!
#include <util/setbaud.h> // avr: helper macros for baud rate calculations

static void sInitRxTx(void)
{
    // disable all while setting baud rate
    UCSR1A = 0x00;
    UCSR1B = 0x00;

    // set baudrate
    UBRR1H = UBRRH_VALUE;
    UBRR1L = UBRRL_VALUE;
#   if USE_2X
    SETBITS(UCSR1A, BIT(U2X1));
#   else
    CLRBITS(UCSR1A, BIT(U2X1));
#   endif

    // set mode to asynchronous USART, no parity, 1 stop bit, 8bit (i.e. 8N1)
    UCSR1C = BIT(UCSZ10) | BIT(UCSZ11);

    // setup send and receive pins (FIXME: necessary?)
    PIN_OUTPUT(_PD3);
    PIN_INPUT(_PD2);
    PIN_PULLUP_ON(_PD2);

    // enable transmitter & receiver
    SETBITS(UCSR1B, BIT(TXEN1) | BIT(RXEN1) );

    // initialise the output buffer
    svTxBufHead = 0;
    svTxBufTail = 0;
    svTxBufSize = 0;
    svTxBufPeak = 0;

    // initialise input buffer
    svRxBufHead = 0;
    svRxBufTail = 0;
    svRxBufSize = 0;
    svRxBufPeak = 0;
    svRxBufDrop = 0;

    // input sync
    osSemaphoreCreate(&sRxReadySem, 0);

    // enable RX complete interrupt
    SETBITS(UCSR1B, BIT(RXCIE1));
}

static void sTxByte(const uint8_t c)
{
    // guarantee output if the scheduler is not running
    if (!osTaskIsSchedulerRunning())
    {
        // we can use the buffer if interrupts are enabled
        if (SREG & BIT(7))
        {
            while (svTxBufSize == sizeof(svTxBuf))
            {
                // busy-wait
            }
        }
        // we must send right away if interrupts are not enabled
        else
        {
            CLRBITS(UCSR1B, BIT(UDRIE1));
            loop_until_bit_is_set(UCSR1A, UDRE1);
            UDR1 = c;
            return;
        }
    }

    // wait for space in buffer
    else
    {
        while (svTxBufSize == sizeof(svTxBuf))
        {
            osTaskDelay(1);
        }
    }

    CS_ENTER;

    svTxBuf[svTxBufHead] = c;
    svTxBufHead += 1;
    svTxBufHead %= sizeof(svTxBuf);
    svTxBufSize++;
    if (svTxBufSize > svTxBufPeak)
    {
        svTxBufPeak = svTxBufSize;
    }
    SETBITS(UCSR1B, BIT(UDRIE0));

    CS_LEAVE;
}

ISR(USART1_UDRE_vect)
{
    osIsrEnter();

    // load next char
    if (svTxBufSize != 0) // (svTxBufHead != svTxBufTail)
    {
        const char c = /*~*/svTxBuf[svTxBufTail];
        svTxBufTail += 1;
        svTxBufTail %= sizeof(svTxBuf);
        svTxBufSize--;
        UDR1 = c;
    }
    else
    {
        CLRBITS(UCSR1B, BIT(UDRIE1));
    }

    osIsrLeave();
}

ISR(USART1_RX_vect)
{
    osIsrEnter();

    const char c = UDR1; // always read this or this ISR will fire continuously

    if ( (svRxBufSize == 0) || (svRxBufHead != svRxBufTail) )
    {
        svRxBuf[svRxBufHead] = c;
        svRxBufHead += 1;
        svRxBufHead %= sizeof(svRxBuf);
        svRxBufSize++;
        if (svRxBufSize > svRxBufPeak)
        {
            svRxBufPeak = svRxBufSize;
        }
    }
    else
    {
        svRxBufDrop++;
    }

    osSemaphoreGive(&sRxReadySem, true);

    osIsrLeave();
}

// reads a character from the rx buffer
static uint8_t sRxByte(void)
{
    // wait until character is available
    while (svRxBufSize == 0)
    {
        // sleep
        if (osTaskIsSchedulerRunning())
        {
            osTaskDelay(1);
        }
        // or busy-wait
    }

    uint8_t b;

    CS_ENTER;

    // get char
    b = svRxBuf[svRxBufTail];
    // move tail
    svRxBufTail += 1;
    svRxBufTail %= sizeof(svRxBuf);
    svRxBufSize--;

    CS_LEAVE;

    return b;
}

// timeout: number of ticks (0 = forever, -1 = don't wait)
static __INLINE uint8_t sRxBufSize(const int32_t timeout)
{
    if (svRxBufSize == 0)
    {
        osSemaphoreTake(&sRxReadySem, timeout);
    }
    return svRxBufSize;
}

static void sBreak(void)
{
    // disable transmit
    CLRBITS(UCSR1B, BIT(TXEN1));
    CLRBITS(UCSR1B, BIT(UDRIE1));

    // configure tx pin as output
    PIN_OUTPUT(_PD3);

    // set pin low for the break period (5 characters)
    PIN_LOW(_PD3);
    osTaskDelay( 1000 / (BAUD / 10 / 5) );

    // and keep it high for some time, too
    PIN_HIGH(_PD3);
    osTaskDelay( 1000 / (BAUD / 10 / 5) );

    // enable transmit
    SETBITS(UCSR1B, BIT(TXEN1));
    SETBITS(UCSR1B, BIT(UDRIE1));
}


/* ***** bluetooth commands ************************************************** */

// forward declarations
static int8_t sParse(const uint8_t *data, const uint8_t size);
static void sHandle(const uint8_t *data);

//! send a request to the LMX9830 chip on the module [LMX9830, pp.117]
/*!
    \param[in] opcode       opcode value (or #LMX_OPCODE_NONE for read-only)
    \param[in] payload      pointer to the payload (or #NULL, with \c size = 0)
    \param[in] size         size of the payload
    \param[in] isProgMem    true if \c payload is in progmem
    \param[in] respTimeout  maximum time [ticks] to wait for response (-1 = don't wait, 0 = forever)

    \return true if the command has successfully completed
*/
static bool sRequest(const LMX_OPCODE_t opcode, const uint8_t *payload, const uint16_t size,
    const bool isProgMem, const int32_t timeout)
{
    const bool readOnly = opcode == LMX_OPCODE_NONE ? true : false;

    // ***** send command *****

    const LMX_PTYPE_t ptype = LMX_PTYPE_REQ;
    if (!readOnly)
    {
        const uint8_t sizeHi = (uint8_t)(size >> 8);
        const uint8_t sizeLo = (uint8_t)size;
        const uint8_t cksum = ptype + opcode + sizeLo + sizeHi;

        DEBUG("ARF32: > 0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"]",
            ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode), size);

        sTxByte(LMX_STX);
        sTxByte(ptype);
        sTxByte(opcode);
        sTxByte(sizeLo);
        sTxByte(sizeHi);
        sTxByte(cksum);

        uint16_t payloadSize = size;
        const uint8_t *pkPayload = payload;
        if (isProgMem)
        {
            while (payloadSize-- != 0)
            {
                //sBtTxByte(*pkPayload++);
                sTxByte(pgm_read_byte(pkPayload));
                pkPayload++;
            }
        }
        else
        {
            while (payloadSize-- != 0)
            {
                sTxByte(*pkPayload);
                pkPayload++;
            }
        }
        sTxByte(LMX_ETX);
    }


    // ***** get response *****

    if (timeout < 0)
    {
        return true;
    }

    // resonse buffer (this also determines the maximum size of a response we can handle)
    static uint8_t resp[250];
    uint32_t endTime = osTaskGetTicks() + (uint32_t)timeout;
    uint8_t respIx = 0;
    bool res = readOnly ? true : false;

    while ( (timeout == 0) || (osTaskGetTicks() < endTime) )
    {
        // wait a bit for data to come in (this is not necessary but will
        // result in fewer calls to sParse() below)
        osTaskDelay(10);

        // receive
        uint8_t n = sRxBufSize(-1);
        if (n == 0)
        {
            continue;
        }
        while (n-- != 0)
        {
            resp[respIx] = sRxByte();
            //DEBUG("recv[%"PRIu8"]=0x%02"PRIx8, respIx, resp[respIx]);
            respIx++;
        }

        // check if we have a complete message
        const int8_t parse = sParse(resp, respIx);
        //DEBUG("parse=%"PRIi8, parse);

        // drop first byte
        if (parse < 0)
        {
            uint8_t ix = 0;
            while (ix < (respIx - 1))
            {
                resp[ix] = resp[ix + 1];
                ix++;
            }
            respIx--;
        }
        // process message
        else if (parse > 0)
        {
            // process all messages
            sHandle(resp);

            // is it the expected response
            if ( ((LMX_PTYPE_t)resp[1] == LMX_PTYPE_CFM) && ((LMX_OPCODE_t)resp[2] == opcode) )
            {
                res = true;
                break;
            }
        }
        // otherwise wait (parse == 0)
    }

    if (!res)
    {
        WARNING("ARF32: timeout 0x%02"PRIx8" %S 0x%02"PRIx8" %S",
            ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode));
    }

    return res;
}

// -1 = nada, 0 = wait, > 0 = size of data
static int8_t sParse(const uint8_t *data, const uint8_t size)
{
    // not a message
    if (data[0] != LMX_STX)
    {
        return -1; // nada
    }

    // header and framing not complete yet
    if (size < LMX_FRAME_SIZE)
    {
        return 0; // wait
    }

    // check header
    LMX_PTYPE_t  ptype  = data[1];
    LMX_OPCODE_t opcode = data[2];
    uint8_t      sizeHi = data[3];
    uint8_t      sizeLo = data[4];
    uint8_t      cksum  = data[5];
    const uint8_t ck    = ptype + opcode + sizeLo + sizeHi;

    if (cksum != ck)
    {
        WARNING("ARF32: checksum mismatch %02"PRIx8" != %02"PRIx8" (0x%02"PRIx8" %S 0x%02"PRIx8" %S)",
            cksum, ck, ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode));
        return -1; // nada
    }

    // wait for payload
    const uint16_t payloadSize = ((uint16_t)sizeLo << 8) | sizeHi;
    const uint16_t messageSize = LMX_FRAME_SIZE + payloadSize;
    if (size < messageSize)
    {
        //DEBUG("wait payload %"PRIu16" %"PRIu16, size, messageSize);
        return 0; // wait
    }

    // check stop byte
    if (data[messageSize - 1] != LMX_ETX)
    {
        return -1; // nada
    }

    DEBUG("ARF32: < 0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"]",
           ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode), payloadSize);

    return messageSize;
}

static void sHandle(const uint8_t *data)
{
    LMX_PTYPE_t  ptype  = data[1];
    LMX_OPCODE_t opcode = data[2];
    uint8_t      sizeHi = data[3];
    uint8_t      sizeLo = data[4];
    const uint16_t size = ((uint16_t)sizeLo << 8) | sizeHi;

    DEBUG("ARF32: = 0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"]",
           ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode), size);

}

/* ***** init **************************************************************** */

void arf32Init(void)
{
    DEBUG("arf32: init");

    sInitRxTx();

}
static void sArf32Task(void *pArg);

void arf32Start(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("arf", 3, &task, stack, sizeof(stack), sArf32Task, NULL);
}


/* ***** task **************************************************************** */

static void sArf32Task(void *pArg)
{
    UNUSED(pArg);

    const char *foo = "foo";
    const char *bar = PSTR("bar");
    NOTICE("foo=%s %p bar=%S %p  -- %p", foo, foo, bar, bar, RAMEND);



    while (ENDLESS)
    {
        DEBUG("arf32... %"PRIu32, osTaskGetTicks());
        {
            uint32_t n = 55555;
            while (n--)
            {
            }
            osTaskDelay(2345);
        }

        {
            DEBUG("send...");
            /*
            //const uint8_t data[] = "blabla\0";
            const uint8_t data[] = { 0x02,   0x52, 0x05,  0x00, 0x00, 0x57,   0x03 };
            for (uint8_t ix = 0; ix < sizeof(data); ix++)
            {
                sTxByte(data[ix]);
                }*/

            const bool res = sRequest(LMX_OPCODE_GAP_READ_LOCAL_BDA, NULL, 0, false, 500);

/*
            uint8_t recv[100];
            uint8_t ix = 0;
            while (ENDLESS)
            {
                uint8_t n = sRxBufSize(1000);
                if (n == 0)
                {
                    DEBUG("no more data");
                    break;
                }
                while (n-- != 0)
                {
                    const uint8_t c = sRxByte();
                    DEBUG("recv[%"PRIu8"]=0x%02"PRIx8, ix, c);
                    recv[ix++] = c;
                }
            }
            //DEBUG("data=%s ix=%"PRIu8" recv=%s", data, ix, recv);
            */
        }
    }
}


/* ***** status ************************************************************** */

// make application status string
void arf32Status(char *str, const size_t size)
{
    UNUSED(str);
    UNUSED(size);
    PRINT_W("mon: arf32: rx=%"PRIu8"/%"PRIu8"/%"PRIu8"/%"PRIu8" tx=%"PRIu8"/%"PRIu8"/%"PRIu8,
        svRxBufSize, svRxBufPeak, (uint8_t)sizeof(svRxBuf), svRxBufDrop,
        svTxBufSize, svTxBufPeak, (uint8_t)sizeof(svRxBuf));
    /*const int n = *//*snprintf_P(str, size,
        PSTR("arf32: rx=%"PRIu8"/%"PRIu8"/%"PRIu8"/%"PRIu8" tx=%"PRIu8"/%"PRIu8"/%"PRIu8),
        svRxBufSize, svRxBufPeak, (uint8_t)sizeof(svRxBuf), svRxBufDrop,
        svTxBufSize, svTxBufPeak, (uint8_t)sizeof(svRxBuf));*/
    svRxBufPeak = 0;
    svRxBufDrop = 0;
    svTxBufPeak = 0;
}


//@}
// eof

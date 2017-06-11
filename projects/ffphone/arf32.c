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

/* ***** serial comms ******************************************************** */

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

static void sArf32InitRxTx(void)
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

    // enable transmitter & receiver
    SETBITS(UCSR1B, BIT(TXEN1) | BIT(RXEN1) );

    // initialise the output buffer
    svTxBufHead = 0;
    svTxBufTail = 0;
    svTxBufSize = 0;
    svTxBufPeak = 0;

    // setup receive pin
    PIN_INPUT(_PE0);
    PIN_PULLUP_ON(_PE0);

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

static void sTxSendData(const uint8_t *data, const uint8_t size)
{
    // add data to tx buffer
    for (uint8_t ix = 0; ix < size; ix++)
    {
        const char c = (const char)data[ix];


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
                continue;
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
static uint8_t sRxGetByte(void)
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
static __INLINE uint8_t sRxGetBufSize(const int32_t timeout)
{
    if (svRxBufSize == 0)
    {
        osSemaphoreTake(&sRxReadySem, timeout);
    }
    return svRxBufSize;
}


/* ***** init **************************************************************** */

void arf32Init(void)
{
    DEBUG("arf32: init");

    sArf32InitRxTx();

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

    while (ENDLESS)
    {
        DEBUG("arf32... %"PRIu32, osTaskGetTicks());
        {
            uint32_t n = 55555;
            while (n--)
            {
            }
            osTaskDelay(1234);
        }

        {
            DEBUG("send...");
            //const uint8_t data[] = "blabla\0";
            const uint8_t data[] = { 0x02, 0x52, 0x05, 0x00, 0x00, 0x57, 0x03 };
            sTxSendData(data, sizeof(data));
            uint8_t recv[100];
            uint8_t ix = 0;
            while (ENDLESS)
            {
                uint8_t n = sRxGetBufSize(1000);
                if (n == 0)
                {
                    DEBUG("no more data");
                    break;
                }
                while (n-- != 0)
                {
                    const uint8_t c = sRxGetByte();
                    DEBUG("recv[%"PRIu8"]=0x%02"PRIx8, ix, c);
                    recv[ix++] = c;
                }
            }
            //DEBUG("data=%s ix=%"PRIu8" recv=%s", data, ix, recv);

        }
    }
}


/* ***** status ************************************************************** */

// make application status string
void arf32Status(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("arf32 status..."));
}


//@}
// eof

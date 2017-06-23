/*!
    \file
    \brief flipflip's Arduino Uno stuff: Adeunis ARF32 bluetooth module (see \ref PROJECTS_FFPHONE_ARF32)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE_ARF32 ARF32
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/io.h>        // avr: AVR device-specific IO definitions
#include <avr/interrupt.h> // avr: interrupt things

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
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
    PIN_OUTPUT(ARF32_TX_PIN);
    PIN_INPUT(ARF32_RX_PIN);
    PIN_PULLUP_ON(ARF32_RX_PIN);

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
    PIN_OUTPUT(ARF32_TX_PIN);

    // set pin low for the break period (5 characters)
    PIN_LOW(ARF32_TX_PIN);
    osTaskDelay( 1000 / (BAUD / 10 / 5) );

    // and keep it high for some time, too
    PIN_HIGH(ARF32_TX_PIN);
    osTaskDelay( 1000 / (BAUD / 10 / 5) );

    // enable transmit
    SETBITS(UCSR1B, BIT(TXEN1));
    SETBITS(UCSR1B, BIT(UDRIE1));
}


/* ***** ARF32 commands and state ******************************************** */

// forward declarations
static int16_t sParse(const uint8_t *data, const uint8_t size);
static void sProcess(uint8_t *data);

//! send a request to the LMX9830 chip on the module [LMX9830, pp.117]
/*!
    \param[in] opcode       opcode value (or #LMX_OPCODE_NONE for read-only)
    \param[in] payload      pointer to the payload (or #NULL, with \c size = 0)
    \param[in] size         size of the payload
    \param[in] isProgMem    true if \c payload is in progmem
    \param[in] respTimeout  maximum time [ticks] to wait for response (-1 = don't wait, 0 = forever)

    \return true if the command has successfully completed
*/
static bool sRequest(const LMX_OPCODE_t reqOpcode, const uint8_t *payload, const uint16_t size,
    const bool isProgMem, const int32_t timeout)
{
    const bool readOnly = reqOpcode == LMX_OPCODE_NONE ? true : false;

    // ***** send command *****

    const LMX_PTYPE_t reqPtype = LMX_PTYPE_REQ;
    if (!readOnly)
    {
        const uint8_t sizeHi = (uint8_t)(size >> 8);
        const uint8_t sizeLo = (uint8_t)size;
        const uint8_t cksum = reqPtype + reqOpcode + sizeLo + sizeHi;

        DEBUG("arf32: > 0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"]",
            reqPtype, lmxGetPtypeString(reqPtype), reqOpcode, lmxGetOpcodeString(reqOpcode), size);

        sTxByte(LMX_STX);
        sTxByte(reqPtype);
        sTxByte(reqOpcode);
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
    uint16_t respIx = 0;

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
        const int16_t parse = sParse(resp, respIx);
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
            sProcess(resp);

            // expected response? (most commands respond with confirming the request)
            const LMX_PTYPE_t  msgPtype  = resp[1];
            const LMX_OPCODE_t msgOpcode = resp[2];
            if ( (msgPtype == LMX_PTYPE_CFM) && (msgOpcode == reqOpcode) )
            {
                const LMX_ERROR_t msgError  = parse > LMX_FRAME_SIZE ? resp[LMX_PAYLOAD_OFFSET] : LMX_ERROR_OK;
                if (msgError == LMX_ERROR_OK)
                {
                    return true;
                }
                else
                {
                    WARNING("arf32: error %S (0x%02"PRIx8" %S 0x%02"PRIx8" %S error)",
                        lmxGetErrorString(msgError),
                        reqPtype, lmxGetPtypeString(reqPtype), reqOpcode, lmxGetOpcodeString(reqOpcode));
                    return false;
                }
            }
            // some commands are different, though...
            else if ( (reqOpcode == LMX_OPCODE_RESET) && (msgPtype == LMX_PTYPE_IND) && (msgOpcode == LMX_OPCODE_DEVICE_READY) )
            {
                return true;
            }
        }
        // otherwise wait (parse == 0)
    }

    if (readOnly)
    {
        return true;
    }
    else
    {
        WARNING("arf32: timeout (0x%02"PRIx8" %S 0x%02"PRIx8" %S)",
            reqPtype, lmxGetPtypeString(reqPtype), reqOpcode, lmxGetOpcodeString(reqOpcode));
        return false;
    }
}

//-------------------------------------------------------------------------------

// -1 = nada, 0 = wait, > 0 = size of data
static int16_t sParse(const uint8_t *data, const uint8_t size)
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
        WARNING("arf32: checksum mismatch %02"PRIx8" != %02"PRIx8" (0x%02"PRIx8" %S 0x%02"PRIx8" %S)",
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

    const char *errorStr = (payloadSize > 0) && (ptype == LMX_PTYPE_CFM) ?
        lmxGetErrorString(data[LMX_PAYLOAD_OFFSET]) : PSTR("n/a");
    DEBUG("arf32: < 0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"] : %S",
        ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode),
        payloadSize, errorStr);

    return (int16_t)messageSize;
}

//-------------------------------------------------------------------------------

typedef struct INFO_s
{
    ARF32_STATE_t arfState;

    // LMX_OPCODE_STORE_SDP_RECORD, LMX_OPCODE_ENABLE_SDP_RECORD
    uint8_t       sdpRecordId;

    // LMX_OPCODE_SPP_TRANSPARENT_MODE, LMX_OPCODE_DEVICE_READY
    LMX_MODE_t    lmxMode;

    // LMX_OPCODE_SPP_INCOMING_LINK_ESTABLISHED
    uint8_t       remoteAddr[6];
    bool          linkChange;

    // LMX_OPCODE_SPP_TRANSPARENT_MODE, LMX_OPCODE_SPP_INCOMING_LINK_ESTABLISHED
    uint8_t       localPort;

} INFO_t;

static INFO_t sInfo;

__INLINE ARF32_STATE_t arf32GetState(void)
{
    return sInfo.arfState;
}

const char *arf32StateStr(const ARF32_STATE_t state)
{
    switch (state)
    {
        case ARF32_STATE_UNKNOWN: return PSTR("UNKNOWN");
        case ARF32_STATE_READY:   return PSTR("READY");
        case ARF32_STATE_PAIRED:  return PSTR("PAIRED");
        case ARF32_STATE_INCALL:  return PSTR("INCALL");
        case ARF32_STATE_ERROR:   return PSTR("ERROR");
    }
    return PSTR("???");
}

static void sProcess(uint8_t *data)
{
    LMX_PTYPE_t  ptype  = data[1];
    LMX_OPCODE_t opcode = data[2];
    uint8_t      sizeHi = data[3];
    uint8_t      sizeLo = data[4];
    const uint16_t size = ((uint16_t)sizeLo << 8) | sizeHi;

    bool processed = true;
    switch (opcode)
    {
        case LMX_OPCODE_DEVICE_READY:
        {
            const uint8_t len = data[LMX_PAYLOAD_OFFSET];
            char *swVerStr = (char *)&data[LMX_PAYLOAD_OFFSET + 1];
            swVerStr[len] = '\0';
            char str[40];
            strcpy(str, swVerStr); // workaround...
            //DEBUG("arf32: device ready, sw version %s", swVerStr); // ...because this crashes avr-gcc 4.9.2
            DEBUG("arf32: device ready, sw version %s", str);
            sInfo.lmxMode = LMX_MODE_COMMAND;
            break;
        }

        case LMX_OPCODE_STORE_SDP_RECORD:
        {
            const uint8_t id = data[LMX_PAYLOAD_OFFSET + 1];
            DEBUG("arf32: SDP record stored, id=%"PRIu8, id);
            sInfo.sdpRecordId = id;
            break;
        }

        case LMX_OPCODE_ENABLE_SDP_RECORD:
        {
            const LMX_ERROR_t error = data[LMX_PAYLOAD_OFFSET];
            if (error == LMX_ERROR_RECORD_ALREADY_IN_SELECTED_STATE)
            {
                data[LMX_PAYLOAD_OFFSET] = LMX_ERROR_OK;
            }
            if (data[LMX_PAYLOAD_OFFSET] == LMX_ERROR_OK)
            {
                DEBUG("arf32: SDP record %"PRIu8" enabled (%S)", data[LMX_PAYLOAD_OFFSET + 1 ],
                    lmxGetErrorString(error));
            }
            break;
        }

        case LMX_OPCODE_GAP_LIST_PAIRED_DEVICES:
        {
            const LMX_ERROR_t error = data[LMX_PAYLOAD_OFFSET];
            if (error == LMX_ERROR_OK)
            {
                const uint8_t count = data[LMX_PAYLOAD_OFFSET + 1];
                for (uint8_t ix = 0; ix < count; ix++)
                {
                    const uint8_t offs = LMX_PAYLOAD_OFFSET + 2 + (ix * 6);
                    DEBUG("arf32: paired device #%"PRIu8": %02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8,
                        ix + 1, data[offs], data[offs + 1], data[offs + 2], data[offs + 3], data[offs + 4], data[offs + 5]);
                }

            }

            break;
        }

        case LMX_OPCODE_GAP_READ_LOCAL_NAME:
        {
            const uint8_t len = data[LMX_PAYLOAD_OFFSET + 1];
            char *deviceName = (char *)&data[LMX_PAYLOAD_OFFSET + 2];
            deviceName[len] = '\0';
            char str[40];
            strcpy(str, deviceName); // workaround...
            //DEBUG("arf32: deviceName=%s", deviceName); // ...because this crashes avr-gcc 4.9.2
            DEBUG("arf32: deviceName=%s", str);
            break;
        }
        case LMX_OPCODE_DELETE_SDP_RECORDS:
        case LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG:
        case LMX_OPCODE_GAP_WRITE_LOCAL_NAME:
        case LMX_OPCODE_GAP_SET_FIXED_PIN:
        case LMX_OPCODE_SET_PORTS_TO_OPEN:
        case LMX_OPCODE_STORE_CLASS_OF_DEVICE:
        case LMX_OPCODE_RESTORE_FACTORY_SETTINGS:
        case LMX_OPCODE_SET_EVENT_FILTER:
        {
            const LMX_ERROR_t error = data[LMX_PAYLOAD_OFFSET];
            DEBUG("arf32: %S (%S)", lmxGetErrorString(error), lmxGetOpcodeString(opcode));
            break;
        }

        case LMX_OPCODE_GAP_ACL_ESTABLISHED:
        case LMX_OPCODE_GAP_ACL_TERMINATED:
        {
            const uint8_t *pkAddr = &data[LMX_PAYLOAD_OFFSET];
            DEBUG("arf32: %S addr=%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8" code=%02"PRIx8,
                lmxGetOpcodeString(opcode),
                pkAddr[0], pkAddr[1], pkAddr[2], pkAddr[3], pkAddr[4], pkAddr[5],
                data[LMX_PAYLOAD_OFFSET + 6]);
            break;
        }

        case LMX_OPCODE_SPP_INCOMING_LINK_ESTABLISHED:
        {
            memcpy(sInfo.remoteAddr, &data[LMX_PAYLOAD_OFFSET], 6);
            sInfo.localPort = data[LMX_PAYLOAD_OFFSET + 6];
            DEBUG("arf32: %S port=%"PRIu8" addr=%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8,
                lmxGetOpcodeString(opcode), sInfo.localPort,
                sInfo.remoteAddr[0], sInfo.remoteAddr[1], sInfo.remoteAddr[2],
                sInfo.remoteAddr[3], sInfo.remoteAddr[4], sInfo.remoteAddr[5]);
            sInfo.linkChange = true;
/*
                sBtRemoteDevice.valid = TRUE;
                sBtRemoteDevice.new = TRUE;
                sBtState = BT_STATE_PAIRED;
*/
                break;
            }
        case LMX_OPCODE_SPP_TRANSPARENT_MODE:
        {
            if (ptype == LMX_PTYPE_IND)
            {
                sInfo.localPort = data[LMX_PAYLOAD_OFFSET];
                sInfo.lmxMode   = data[LMX_PAYLOAD_OFFSET + 1];
                DEBUG("arf32: %S port=%"PRIu8" mode=%S",
                    lmxGetOpcodeString(opcode), sInfo.localPort, lmxGetModeString(sInfo.lmxMode));
            }
            else if (ptype == LMX_PTYPE_CFM)
            {
                const LMX_ERROR_t error = data[LMX_PAYLOAD_OFFSET];
                if (error == LMX_ERROR_OK)
                {
                    sInfo.localPort = data[LMX_PAYLOAD_OFFSET + 1];
                    sInfo.lmxMode   = LMX_MODE_TRANSPARENT;
                    DEBUG("arf32: %S port=%"PRIu8" mode=%S",
                        lmxGetOpcodeString(opcode), sInfo.localPort, lmxGetModeString(sInfo.lmxMode));
                }
            }
            else
            {
                processed = false;
            }
            break;
        }
/*

        case LMX_OPCODE_SPP_LINK_RELEASED:
            if ( (ptype == LMX_PTYPE_IND) && (payloadSize > 1) )
            {
                DEBUG1("BT: = RFCOMM 0x%02x link release 0x%02x %S",
                       pkPayload[1], pkPayload[0], lmxGetLinkreleaseString(pkPayload[0]));
                res = TRUE;
                error = LMX_ERROR_OK;
                sBtState = BT_STATE_READY;
            }
            break;


        case LMX_OPCODE_GAP_REMOTE_DEVICE_NAME:
            if ( (ptype == LMX_PTYPE_CFM) && (payloadSize > 7) )
            {
                error = pkPayload[0];
                res = TRUE;
                if (memcmp(&sBtRemoteDevice.addr, &pkPayload[1], 6) == 0)
                {
                    if (pkPayload[7])
                    {
                        DEBUG1("BT: = remote device name: %s", &pkPayload[8]);
                    }
                    else
                    {
                        DEBUG1("BT: = unknown remote device name");
                    }
                }
            }
            break;

            // not sure why we see this message sometimes
        case LMX_OPCODE_SPP_SEND_DATA:
            if ( (ptype == LMX_PTYPE_CFM) && (payloadSize > 1) )
            {
                error = pkPayload[0];
                res = TRUE;
                DEBUG1("BT: = RFCOMM 0x%02x", pkPayload[1]);
            }
            break;
*/

        default:
            processed = false;
            break;
    }

    if (!processed)
    {
        WARNING("arf32: unprocessed message (0x%02"PRIx8" %S 0x%02"PRIx8" %S [%"PRIu16"])",
            ptype, lmxGetPtypeString(ptype), opcode, lmxGetOpcodeString(opcode), size);
    }
}

//-------------------------------------------------------------------------------

static bool sResetAndReconfigure(void)
{
    PRINT("arf32: reset & reconfigure");

    // flush input buffer
    sRequest(LMX_OPCODE_NONE, NULL, 0, false, 50);

    // send break to put the device into command mode
    sBreak();

    // read from the device for a while..
    sRequest(LMX_OPCODE_NONE, NULL, 0, false, 500);

    // parameters for the configuration requests below
    static const uint8_t skAudioConfig[] PROGMEM =
    {
        LMX_AUDIOCODEC_MOTOROLA, LMX_AIRFORMAT_CVSD // FIXME: right?
    };
    static const uint8_t skEventFilter[] PROGMEM =
    {
        LMX_EVENTFILTER_ALL // report all
    };
#if 0
    static const uint8_t skSdpRecord[] PROGMEM =
    {
        0x01, // RFCOMM port 1
        LMX_AUTH_INOUT, // authentication setting
        LMX_ENC_INOUT,  // encryption setting
        0x54, // SDP record size
        0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x35, 0x06, 0x19, 0x1e, 0x11, 0x19, 0x03, 0x12, 0x04, 0x00,
        0x35, 0x0c, 0x35, 0x03, 0x19, 0x00, 0x01, 0x35, 0x05, 0x19, 0x03, 0x00,
        0x08, 0x01, 0x05, 0x00, 0x35, 0x03, 0x19, 0x02, 0x10, 0x06, 0x00, 0x35,
        0x09, 0x09, 0x6e, 0x65, 0x09, 0x6a, 0x00, 0x09, 0x00, 0x01, 0x09, 0x00,
        0x35, 0x08, 0x35, 0x06, 0x19, 0x1e, 0x11, 0x09,
        0x01, 0x05, // HFP version (it seems that Android wants 1.05, while SimplyBlueCrap suggests 1.01)
        0x00, 0x01,
        0x25, 0x0a, 0x48, 0x61, 0x6e, 0x64, 0x73, 0x66, 0x72, 0x65, 0x65, 0x00,
        0x11, 0x03, 0x09,
        0x1f, // 0x1f = features (0x01 = EC/NR, 0x02 = Call waiting / 3way calling, 0x04 = CLI presentation cap, 0x08 = voice recognition activation, 0x10 = remote volume control)
        0x00
    };
    static const uint8_t skClassOfDevice[] PROGMEM =
    {
        0x08, 0x04, 0x22 // CoD handsfree 220408
    };
#else
    static const uint8_t skSdpRecord[] PROGMEM =
    {
        0x01, // RFCOMM port 1
        LMX_AUTH_INOUT, // authentication setting
        LMX_ENC_INOUT,  // encryption setting
        0x51, 0x00,     // SDP record size
        // SDP record headset
        0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x35, 0x06, 0x19, 0x08, 0x11, 0x19, 0x03, // 16
        0x12, 0x04, 0x00, 0x35, 0x0c, 0x35, 0x03, 0x19, 0x00, 0x01, 0x35, 0x05, 0x19, 0x03, 0x00, 0x08, // 32
        0x01, 0x05, 0x00, 0x35, 0x03, 0x19, 0x02, 0x10, 0x06, 0x00, 0x35, 0x09, 0x09, 0x6e, 0x65, 0x09, // 48
        0x6a, 0x00, 0x09, 0x00, 0x01, 0x09, 0x00, 0x35, 0x08, 0x35, 0x06, 0x19, 0x08, 0x11, 0x09, 0x00, // 64
        0x01, 0x00, 0x01, 0x25, 0x08, 0x48, 0x65, 0x61, 0x64, 0x73, 0x65, 0x74, 0x00, 0x02, 0x03, 0x28, // 80
        0x01                                                                                            // 81 = 0x51
    };
    static const uint8_t skClassOfDevice[] PROGMEM =
    {
        0x04, 0x04, 0x22 // CoD headset 220404
    };
#endif
    static const uint8_t skPortsToOpen[] PROGMEM =
    {
        0x01, 0x00, 0x00, 0x00 // bitmask, RFCOMM port 1
    };
    static const uint8_t skDeviceName[] PROGMEM =
    {
        14, 'f', 'l', 'i', 'p', 'f', 'l', 'i', 'p', 'P', 'H', 'O', 'N', 'E', '\0'
    };
    static const uint8_t skPinCode[] PROGMEM =
    {
        4, '1', '2', '3', '4'
    };

    // reset the chip
    if (!sRequest(LMX_OPCODE_RESET, NULL, 0, false, 1000))
    {
        return false;
    }

    // clear all SDP (local service database) records
    if (!sRequest(LMX_OPCODE_DELETE_SDP_RECORDS, NULL, 0, false, 250))
    {
        return false;
    }

    // store default audio config
    if (!sRequest(LMX_OPCODE_SET_DEFAULT_AUDIO_CONFIG, skAudioConfig, sizeof(skAudioConfig), true, 250))
    {
        return false;
    }

    // configure event filter
    if (!sRequest(LMX_OPCODE_SET_EVENT_FILTER, skEventFilter, sizeof(skEventFilter), true, 250))
    {
        return false;
    }

    // store for hands-free profile SDP record
    if (!sRequest(LMX_OPCODE_STORE_SDP_RECORD, skSdpRecord, sizeof(skSdpRecord), true, 250))
    {
        return false;
    }

#if 0
    // enable configured SDP record FIXME: probably not needed
    uint8_t enaSdpRecord[2];
    enaSdpRecord[0] = LMX_SDPSTATE_ENABLE;
    enaSdpRecord[1] = sInfo.sdpRecordId;
    if (!sRequest(LMX_OPCODE_ENABLE_SDP_RECORD, enaSdpRecord, sizeof(enaSdpRecord), false, 250))
    {
        return false;
    }
#endif

    // open ports
    if (!sRequest(LMX_OPCODE_SET_PORTS_TO_OPEN, skPortsToOpen, sizeof(skPortsToOpen), true, 250))
    {
        return false;
    }

    // set class of device
    if (!sRequest(LMX_OPCODE_STORE_CLASS_OF_DEVICE, skClassOfDevice, sizeof(skClassOfDevice), true, 250))
    {
        return false;
    }

    // store local device name
    if (!sRequest(LMX_OPCODE_GAP_WRITE_LOCAL_NAME, skDeviceName, sizeof(skDeviceName), true, 250))
    {
        return false;
    }

    // set PIN code
    if (!sRequest(LMX_OPCODE_GAP_SET_FIXED_PIN, skPinCode, sizeof(skPinCode), true, 250))
    {
        return false;
    }

    // collect some information
    if (!sRequest(LMX_OPCODE_GAP_READ_LOCAL_NAME, NULL, 0, false, 250) ||
        !sRequest(LMX_OPCODE_GAP_LIST_PAIRED_DEVICES, NULL, 0, false, 250))
    {
        return false;
    }

    PRINT("arf32: configuration successful");

    return true;
}

static void sFactoryReset(void)
{
    PRINT("arf32: factory reset");

    // wait a while and then try configuring it again
    DEBUG("arf32: hardware reset");
    PIN_LOW(ARF32_RESET_PIN);
    osTaskDelay(250);
    PIN_HIGH(ARF32_RESET_PIN);
    osTaskDelay(250);

    sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);

    sRequest(LMX_OPCODE_RESTORE_FACTORY_SETTINGS, NULL, 0, false, 5000);

    sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);
}


/* ***** init **************************************************************** */

void arf32Init(void)
{
    DEBUG("arf32: init");

    sInitRxTx();

    PIN_OUTPUT(ARF32_RESET_PIN);
    PIN_HIGH(ARF32_RESET_PIN);

    sInfo.arfState = ARF32_STATE_UNKNOWN;
}


/* ***** task **************************************************************** */

static void sArf32Task(void *pArg)
{
    UNUSED(pArg);

    // keep the ARF32 module going...
    while (ENDLESS)
    {
        {
            static ARF32_STATE_t sLastState;
            if (sLastState != sInfo.arfState)
            {
                PRINT("ARF32 %S -> %S", arf32StateStr(sLastState), arf32StateStr(sInfo.arfState));
                sLastState = sInfo.arfState;
            }
        }

        switch (sInfo.arfState)
        {

            // reset and reconfigure device
            case ARF32_STATE_UNKNOWN:
            {
                memset(&sInfo, 0, sizeof(sInfo));
                sInfo.arfState = ARF32_STATE_UNKNOWN;
                sInfo.lmxMode  = LMX_MODE_UNKNOWN;
                const bool res = sResetAndReconfigure();
                if (res)
                {
                    sInfo.arfState = ARF32_STATE_READY;
                }
                else
                {
                    sInfo.arfState = ARF32_STATE_ERROR;
                }
                break;
            }

            // wait for connection
            case ARF32_STATE_READY:
            {
                sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);
                // check connection
                if (sInfo.linkChange)
                {
                    sInfo.linkChange = false;

                    // FIXME: these don't work...
                    sRequest(LMX_OPCODE_GAP_REMOTE_DEVICE_NAME, sInfo.remoteAddr, 6, false, 1000);
                    sRequest(LMX_OPCODE_READ_RSSI, sInfo.remoteAddr, 6, false, 1000);

                    sInfo.arfState = ARF32_STATE_PAIRED;
                }
                break;
            }

            case ARF32_STATE_PAIRED:
            {
                sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);
                break;
            }

            case ARF32_STATE_INCALL:
            {
                sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);
                break;
            }

            case ARF32_STATE_ERROR:
            {
                ERROR("arf32: ouch! please wait...");
                uint8_t n = 10;
                while (n-- != 0)
                {
                    sRequest(LMX_OPCODE_NONE, NULL, 0, false, 1000);
                }
                sFactoryReset();
                sInfo.arfState = ARF32_STATE_UNKNOWN;
                break;
            }
        }

        // process incoming data sent by the ARF32 spontaneously (e.g. LMX_PTYPE_IND / LMX_OPCODE_DEVICE_READY)
        sRequest(LMX_OPCODE_NONE, NULL, 0, false, 10);
    }
}

void arf32Start(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("arf", 4, &task, stack, sizeof(stack), sArf32Task, NULL);
}


/* ***** status ************************************************************** */

// make application status string
void arf32Status(char *str, const size_t size)
{
    snprintf_P(str, size,
        PSTR("rx=%"PRIu8"/%"PRIu8"/%"PRIu8"/%"PRIu8" tx=%"PRIu8"/%"PRIu8"/%"PRIu8
            " state=%S mode=%S remote=%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8":%02"PRIx8),
        svRxBufSize, svRxBufPeak, (uint8_t)sizeof(svRxBuf), svRxBufDrop,
        svTxBufSize, svTxBufPeak, (uint8_t)sizeof(svRxBuf),
        arf32StateStr(sInfo.arfState), lmxGetModeString(sInfo.lmxMode),
        sInfo.remoteAddr[0], sInfo.remoteAddr[1], sInfo.remoteAddr[2],
        sInfo.remoteAddr[3], sInfo.remoteAddr[4], sInfo.remoteAddr[5]);
    svRxBufPeak = 0;
    svRxBufDrop = 0;
    svTxBufPeak = 0;
}


//@}
// eof

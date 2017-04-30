/*!
    \file
    \brief flipflip's Arduino Uno stuff: u-blox binary protocol (see \ref UBX)

    - Copyright (c) 2014-2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup UBX
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging

#include "ubx.h"

#define UBX_NAV_ANY_USE  (FF_UBX_NAV_PVT_USE || FF_UBX_NAV_AOPSTATUS_USE || FF_UBX_NAV_CLOCK_USE || FF_UBX_NAV_POSECEF_USE || FF_UBX_NAV_VELECEF_USE || FF_UBX_NAV_STATUS_USE) // use any UBX-NAV-* message?
#define UBX_INF_ANY_USE  (FF_UBX_INF_ERROR_USE || FF_UBX_INF_ERROR_USE || FF_UBX_INF_ERROR_USE || FF_UBX_INF_ERROR_USE) // use any UBX-INF-* message?

#if (!UBX_NAV_ANY_USE && !UBX_INF_ANY_USE)
#  error no UBX_*_*_USE enabled
#endif


/* ***** parser ************************************************************* */

#define UBX_SYNC_CHAR_1 0xb5 // UBX protocol framing sync char 1
#define UBX_SYNC_CHAR_2 0x62 // UBX protocol framing sync char 1

// UBX protocol framing header
typedef struct UBX_HEAD_s
{
    uint8_t  sync1;  // sync char 1 (#UBX_SYNC_CHAR_1)
    uint8_t  sync2;  // sync char 1 (#UBX_SYNC_CHAR_1)
    uint8_t  cls;    // message class
    uint8_t  id;     // message id
    uint16_t len;    // payload length
} __PACKED UBX_HEAD_t;

// UBX protocol framing tail
typedef struct UBX_TAIL_s
{
    uint8_t ckA;    // checksum char 1
    uint8_t ckB;    // checksum char 2
} __PACKED UBX_TAIL_t;

#define UBX_HEAD_SIZE sizeof(UBX_HEAD_t) // UBX protocol framing header size
#define UBX_TAIL_SIZE sizeof(UBX_TAIL_t) // UBX protocol framing tail size
#define UBX_FRAME_SIZE (UBX_HEAD_SIZE + UBX_TAIL_SIZE) // UBX protocol framing size


// verify checksum of a UBX message
static bool sUbxChecksum(const uint8_t *pkPayload, uint16_t payloadSize)
{
    uint8_t ckA = 0;
    uint8_t ckB = 0;

    while (payloadSize--)
    {
        ckA += *pkPayload++;
        ckB += ckA;
    }

    return ( (ckA == pkPayload[0]) && (ckB == pkPayload[1]) ) ? true : false;
}


#define UBX_PARSER_BUF_SIZE (uint16_t)(UBX_FRAME_SIZE + UBX_MAX_PAYLOAD_SIZE) // parser buffer size, large enough to fit the largest message we are interested in

static uint8_t sUbxParserBuf[UBX_PARSER_BUF_SIZE] __ALIGN(4); // parser buffer
static uint16_t sUbxParserBufSize = 0; // current size of the parser buffer (write offset)
static uint16_t sUbxParserBufPeak = 0;

static uint32_t sUbxBytes   = 0; // number of bytes received
static uint32_t sUbxDropped = 0; // number of bytes dropped (too big messages, other protocols, or garbage)
static uint32_t sUbxMsgs    = 0; // number of messages received
static uint32_t sUbxWanted  = 0; // number of interesting messages received

void ubxStatus(char *str, const uint8_t size)
{
    snprintf_P(str, size, PSTR("bytes=%"PRIu32" (%"PRIu32") count=%"PRIu32"/%"PRIu32" buf=%"PRIu16"/%"PRIu16"/%"PRIu16),
             sUbxBytes, sUbxDropped, sUbxMsgs, sUbxWanted, sUbxParserBufSize, sUbxParserBufPeak, UBX_PARSER_BUF_SIZE);
    str[size-1] = '\0';
    sUbxParserBufPeak = 0;
}

const UBX_PAYLOADS_t *ubxParse(uint8_t c, UBX_MSG_CLS_t *pMsgCls, UBX_MSG_ID_t *pMsgId)
{
    // concept:
    // - add byte to the input buffer
    // - look for sync chars at the beginning of the buffer
    //   - not there --> discard one byte and start over
    // - wait for rest of framing header
    // - wait for payload (or discard one byte if header size field indicates a too long message)
    // - check checksum
    //   - failed -> discard one byte and start over
    // - copy and remove message from buffer
    // This concept allows for multiple parsers to run on the same buffer. So one could
    // easily add a second parser that looks for NMEA sentences. The implementation
    // below, however, is optimised for UBX only parsing.

    sUbxBytes++;

    // buffer full
    if (sUbxParserBufSize >= UBX_PARSER_BUF_SIZE)
    {
        // drop one byte
        sUbxDropped++;
        sUbxParserBufSize--;
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], sUbxParserBufSize);
    }

    // add character to the buffer
    sUbxParserBuf[sUbxParserBufSize++] = c;

    // track maximum buffer size used
    if (sUbxParserBufSize > sUbxParserBufPeak)
    {
        sUbxParserBufPeak = sUbxParserBufSize;
    }

    // cannot tell with only one byte in the buffer
    if (sUbxParserBufSize < 2)
    {
        // --> WAIT
        return NULL;
    }

    // need the correct sync chars or
    if ( (sUbxParserBuf[0] != UBX_SYNC_CHAR_1) ||
         (sUbxParserBuf[1] != UBX_SYNC_CHAR_2) )
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufSize--;
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], sUbxParserBufSize);
        return NULL;
    }

    // need four more chars (message class and id, payload size)
    if (sUbxParserBufSize < UBX_HEAD_SIZE)
    {
        // --> WAIT
        return NULL;
    }

    // let's see what we have
    const UBX_HEAD_t *pkHead = (const UBX_HEAD_t *)sUbxParserBuf;
    // it's either too big or something else that looks like a UBX message
    if (pkHead->len > UBX_MAX_PAYLOAD_SIZE)
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufSize--;
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], sUbxParserBufSize);
        return NULL;
    }

    // now need to wait for the rest of the payload and the checksum
    const uint16_t size = (pkHead->len + UBX_FRAME_SIZE);
    if (sUbxParserBufSize < size)
    {
        // --> WAIT
        return NULL;
    }

    // and check the checksum
    if (sUbxChecksum(&sUbxParserBuf[UBX_TAIL_SIZE],
                     pkHead->len + UBX_HEAD_SIZE - UBX_TAIL_SIZE) == false)
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufSize--;
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], sUbxParserBufSize);
        return false;
    }

    // have now a valid UBX protocol message
    sUbxMsgs++;

    // so should have a UBX message now... see if it's interesting
    bool interesting = false;

    switch (pkHead->cls)
    {
#if UBX_NAV_ANY_USE
        case UBX_MSG_CLS_NAV:
            switch (pkHead->id)
            {
#  if (FF_UBX_NAV_PVT_USE > 0)
                case UBX_MSG_ID_NAV_PVT:
#  endif
#  if (FF_UBX_NAV_AOPSTATUS_USE > 0)
                case UBX_MSG_ID_NAV_AOPSTATUS:
#  endif
#  if (FF_UBX_NAV_CLOCK_USE > 0)
                case UBX_MSG_ID_NAV_CLOCK:
#  endif
#  if (FF_UBX_NAV_POSECEF_USE > 0)
                case UBX_MSG_ID_NAV_POSECEF:
#  endif
#  if (FF_UBX_NAV_VELECEF_USE > 0)
                case UBX_MSG_ID_NAV_VELECEF:
#  endif
#  if (FF_UBX_NAV_STATUS_USE > 0)
                case UBX_MSG_ID_NAV_STATUS:
#  endif
                    interesting = true;
                    break;
                default:
                    break;
            }
            break;
#endif // UBX_NAV_ANY_USE
#if UBX_INF_ANY_USE
        case UBX_MSG_CLS_INF:
            if (pkHead->len < UBX_INF_MAX_LEN)
            {
                // nul terminate inf message
                char *pStr = (char *)&sUbxParserBuf[sizeof(*pkHead)];
                pStr[pkHead->len] = '\0';
                switch (pkHead->id)
                {
#  if (FF_UBX_INF_ERROR_USE > 0)
                    case UBX_MSG_ID_INF_ERROR:
#    if (FF_UBX_INF_ERROR_USE > 1)
                        ERROR("ubx: %s", pStr);
#    endif
#  endif
#  if (FF_UBX_INF_WARNING_USE > 0)
                    case UBX_MSG_ID_INF_WARNING:
#    if (FF_UBX_INF_WARNING_USE > 1)
                        WARNING("ubx: %s", pStr);
#    endif
#  endif
#  if (FF_UBX_INF_NOTICE_USE > 0)
                    case UBX_MSG_ID_INF_NOTICE:
#    if (FF_UBX_INF_NOTICE_USE > 1)
                        PRINT("ubx: %s", pStr);
#    endif
#  endif
#  if (FF_UBX_INF_TEST_USE > 0)
                    case UBX_MSG_ID_INF_TEST:
#    if (FF_UBX_INF_TEST_USE > 1)
                        NOTICE("ubx: %s", pStr);
#    endif
#  endif
#  if (FF_UBX_INF_DEBUG_USE > 0)
                    case UBX_MSG_ID_INF_DEBUG:
#    if (FF_UBX_INF_DEBUG_USE > 1)
                        DEBUG("ubx: %s", pStr);
#    endif
#  endif
                        interesting = true;
                        break;
                    default:
                        break;
                }
            }
            break;
#endif // UBX_INF_ANY_USE
        default:
            break;
    }

    //DEBUG("ubx: 0x%02"PRIx8" 0x%02"PRIx8" %c", head.cls, head.id, interesting ? 'Y' : 'N');

    // reset parser for next iteration
    sUbxParserBufSize = 0;

    // prepare message payload for output
    if (interesting)
    {
        sUbxWanted++;
        *pMsgCls = pkHead->cls;
        *pMsgId  = pkHead->id;

        // move payload to beginning of buffer (so that's it's aligned)
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[sizeof(UBX_HEAD_t)], pkHead->len);

        return (const UBX_PAYLOADS_t *)sUbxParserBuf;

    }
    // a valid UBX message, but not an interesting one
    else
    {
        return NULL;
    }
}






/* ************************************************************************** */

void ubxInit(void)
{
    DEBUG("ubx: init (payload %"PRIu16", buf %"PRIu16")",
        (uint16_t)sizeof(UBX_PAYLOADS_t), UBX_PARSER_BUF_SIZE);
    memset(sUbxParserBuf, 0, sizeof(sUbxParserBuf));
}




/* ************************************************************************** */

//@}
// eof

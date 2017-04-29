// $Id: ubx.c 3196 2014-10-16 21:14:08Z flip $
// $HeadURL: svn+ssh://fry/flipflipclock/src/ubx.c $
/*!
    \file
    \brief fLiPfLiPcLoCk u-blox stuff implementation (see \ref UBX)

    - Copyright (c) 2014 Philippe Kehl < phkehl at gmail dot com >

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
} __PACKED  UBX_HEAD_t;

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
/*!
    Verifies the checksum of a UBX message.

    \param pkPayload    pointer to the payload data
    \param payloadSize  size of the payload

    It assumes the two checksum characters follow the payload.

    \returns #TRUE if checksum matches, #FALSE otherwise
*/
static bool sUbxChecksum(const uint8_t *pkPayload, uint16_t payloadSize)
{
    uint8_t ckA = 0;
    uint8_t ckB = 0;

    while (payloadSize--)
    {
        ckA += *pkPayload++;
        ckB += ckA;
    }

    return ( (ckA == pkPayload[0]) && (ckB == pkPayload[1]) ) ? TRUE : FALSE;
}


#define UBX_PARSER_BUF_SIZE (uint16_t)(UBX_FRAME_SIZE + UBX_MAX_PAYLOAD_SIZE) // parser buffer size, large enough to fit the largest message we are interested in
#define UBX_PARSER_BUF_XTRA (uint16_t)20 // extra parser buffer bytes used to minimise the memmove() calls in the parser

static uint8_t sUbxParserBuf[UBX_PARSER_BUF_SIZE + UBX_PARSER_BUF_XTRA]; // parser buffer
static uint16_t sUbxParserBufSize = 0; // current size of the parser buffer (write offset)
static uint16_t sUbxParserBufOffs = 0; // current offset into the parser buffer (optimisation, see #UBX_PARSER_BUF_XTRA)
static uint16_t sUbxParserBufPeak = 0;

static uint32_t sUbxBytes   = 0; // number of bytes received
static uint32_t sUbxDropped = 0; // number of bytes dropped (too big messages, other protocols, or garbage)
static uint32_t sUbxMsgs    = 0; // number of messages received
static uint32_t sUbxWanted  = 0; // number of interesting messages received

void ubxStatus(char *str, const uint8_t size)
{
    snprintf_P(str, size, PSTR("bytes=%"PRIu32"/%"PRIu32 " msgs=%"PRIu32"/%"PRIu32" buf=%"PRIu16"/%"PRIu16"/%"PRIu16),
             sUbxBytes, sUbxDropped, sUbxMsgs, sUbxWanted, sUbxParserBufSize, sUbxParserBufPeak, UBX_PARSER_BUF_SIZE);
    str[size-1] = '\0';
    sUbxParserBufPeak = 0;
}

// FIXME: the "XTRA" stuff doesn't seem to help optimising it.. so memmove() isn't that expensive..

bool ubxFeedByte(uint8_t c, UBX_MSG_CLS_t *pMsgCls, UBX_MSG_ID_t *pMsgId, UBX_PAYLOADS_t *pPayload)
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

    // remove discarded data from the buffer, in chunks of #UBX_PARSER_BUF_EXTRA bytes
    if (sUbxParserBufOffs >= UBX_PARSER_BUF_XTRA)
    {
        memmove(&sUbxParserBuf[0], &sUbxParserBuf[sUbxParserBufOffs], sUbxParserBufSize);
        sUbxParserBufOffs = 0;
    }

    // buffer full
    if (sUbxParserBufSize >= UBX_PARSER_BUF_SIZE)
    {
        // drop one byte
        sUbxParserBufOffs++;
        sUbxParserBufSize--;
    }

    // add character to the buffer
    sUbxParserBuf[sUbxParserBufOffs + sUbxParserBufSize++] = c;

    // track maximum buffer size used
    if (sUbxParserBufSize > sUbxParserBufPeak)
    {
        sUbxParserBufPeak = sUbxParserBufSize;
    }

    // cannot tell with only one byte in the buffer
    if (sUbxParserBufSize < 2)
    {
        // --> WAIT
        return FALSE;
    }

    // need the correct sync chars or
    if ( (sUbxParserBuf[sUbxParserBufOffs + 0] != UBX_SYNC_CHAR_1) ||
         (sUbxParserBuf[sUbxParserBufOffs + 1] != UBX_SYNC_CHAR_2) )
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufOffs++;
        sUbxParserBufSize--;
        //memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], --sUbxParserBufSize);
        return FALSE;
    }

    // need four more chars (message class and id, payload size)
    if (sUbxParserBufSize < UBX_HEAD_SIZE)
    {
        // --> WAIT
        return FALSE;
    }

    // let's see what we have
    UBX_HEAD_t head;
    memcpy(&head, &sUbxParserBuf[sUbxParserBufOffs + 0], sizeof(head));
    // it's either too big or something else that looks like a UBX message
    if (head.len > UBX_MAX_PAYLOAD_SIZE)
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufOffs++;
        sUbxParserBufSize--;
        //memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], --sUbxParserBufSize);
        return FALSE;
    }

    // now need to wait for the rest of the payload and the checksum
    const uint16_t size = (head.len + UBX_FRAME_SIZE);
    if (sUbxParserBufSize < size)
    {
        // --> WAIT
        return FALSE;
    }

    // and check the checksum
    if (sUbxChecksum(&sUbxParserBuf[sUbxParserBufOffs + UBX_TAIL_SIZE],
                     head.len + UBX_HEAD_SIZE - UBX_TAIL_SIZE) == FALSE)
    {
        // --> NADA
        sUbxDropped++;
        sUbxParserBufOffs++;
        sUbxParserBufSize--;
        //memmove(&sUbxParserBuf[0], &sUbxParserBuf[1], --sUbxParserBufSize);
        return FALSE;
    }

    // have now a valid UBX protocol message
    sUbxMsgs++;

    // so should have a UBX message now... see if it's interesting
    bool interesting = FALSE;

    switch ((UBX_MSG_CLS_t)head.cls)
    {
#if UBX_NAV_ANY_USE
        case UBX_MSG_CLS_NAV:
            switch ((UBX_MSG_ID_t)head.id)
            {
#  if FF_UBX_NAV_PVT_USE
                case UBX_MSG_ID_NAV_PVT:
#  endif
#  if FF_UBX_NAV_AOPSTATUS_USE
                case UBX_MSG_ID_NAV_AOPSTATUS:
#  endif
#  if FF_UBX_NAV_CLOCK_USE
                case UBX_MSG_ID_NAV_CLOCK:
#  endif
#  if FF_UBX_NAV_POSECEF_USE
                case UBX_MSG_ID_NAV_POSECEF:
#  endif
#  if FF_UBX_NAV_VELECEF_USE
                case UBX_MSG_ID_NAV_VELECEF:
#  endif
#  if FF_UBX_NAV_STATUS_USE
                case UBX_MSG_ID_NAV_STATUS:
#  endif
                    interesting = TRUE;
                    break;
                default:
                    break;
            }
            break;
#endif // UBX_NAV_ANY_USE
#if UBX_INF_ANY_USE
        case UBX_MSG_CLS_INF:
            if (head.len < UBX_INF_MAX_LEN)
            {
                switch ((UBX_MSG_ID_t)head.id)
                {
#  if FF_UBX_INF_ERROR_USE
                    case UBX_MSG_ID_INF_ERROR:
#  endif
#  if FF_UBX_INF_WARNING_USE
                    case UBX_MSG_ID_INF_WARNING:
#  endif
#  if FF_UBX_INF_NOTICE_USE
                    case UBX_MSG_ID_INF_NOTICE:
#  endif
#  if FF_UBX_INF_TEST_USE
                    case UBX_MSG_ID_INF_TEST:
#  endif
#  if FF_UBX_INF_DEBUG_USE
                    case UBX_MSG_ID_INF_DEBUG:
#  endif
                        interesting = TRUE;
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

    // copy message to output
    if (interesting)
    {
        sUbxWanted++;
        *pMsgCls = head.cls;
        *pMsgId  = head.id;
        if (pPayload)
        {
            memcpy(pPayload, &sUbxParserBuf[sUbxParserBufOffs + UBX_HEAD_SIZE], head.len);
#if UBX_INF_ANY_USE
            if ((UBX_MSG_CLS_t)head.cls == UBX_MSG_CLS_INF)
            {
                pPayload->infAny.str[head.len] = '\0';
            }
#endif
        }
    }

    // remove message from buffer
    memmove(&sUbxParserBuf[0], &sUbxParserBuf[sUbxParserBufOffs + size], sUbxParserBufSize - size);
    sUbxParserBufSize -= size;
    sUbxParserBufOffs = 0;

    return interesting;
}






/* ************************************************************************** */

void ubxInit(void)
{
    DEBUG("ubx: init (buf %"PRIu16")", UBX_PARSER_BUF_SIZE);
    memset(sUbxParserBuf, 0, sizeof(sUbxParserBuf));
}




/* ************************************************************************** */

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: GNSS receiver abstraction (see \ref GNSS)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup GNSS
    @{

    \todo use timepulse output to determine age of epoch
    \todo implement epoch collection from multiple UBX-NAV-*
*/

#include <string.h>        // libc: string operations
#include <stdlib.h>        // libc: standard library definitions
#include <stdio.h>         // libc: standard buffered input/output
#include <inttypes.h>      // libc: fixed size integer types

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "ubx.h"           // ff: u-blox binary protocol

#include "gnss.h"

// check configuration
#if (FF_GNSS_TASK_PRIO >= FF_SYS_TASK_PRIO)
#  warning FF_GNSS_TASK_PRIO >= FF_SYS_TASK_PRIO
#endif
#if ( (FF_GNSS_PARSER < 0) || (FF_GNSS_PARSER > 1) )
#  error illegal value for FF_GNSS_PARSER
#endif
#if (FF_GNSS_PARSER == 1)
#  if (!FF_UBX_NAV_PVT_USE)
#    error FF_UBX_NAV_PVT_USE not enabled
#  endif
#endif

//--------------------------------------------------------------------------------------------------

// epoch data collecting
static GNSS_EPOCH_t sEpoch;
uint32_t sMsssLastNofix;
uint32_t sMsssLastEpoch;
//#if (FF_GNSS_PARSER == 1)
//static uint32_t sCollectItow;
//bool sUbxNavPvtSeen;
//bool sUbxNavStatusSeen;
//#endif

static OS_MUTEX_t     sEpochMx;     // access protection for sEpoch
static OS_SEMAPHORE_t sNewEpochSem; // new time solution available semaphore

bool gnssGetEpoch(GNSS_EPOCH_t *pEpoch, int32_t timeout)
{
    if (timeout >= 0)
    {
        if (osSemaphoreTake(&sNewEpochSem, timeout) == false)
        {
            return false;
        }
    }

    if (pEpoch != NULL)
    {
        osMutexClaim(&sEpochMx, 0);
        memcpy(pEpoch, &sEpoch, sizeof(*pEpoch));
        osMutexRelease(&sEpochMx);
    }

    return true;
}

//--------------------------------------------------------------------------------------------------

// forward declarations
static void sGnssTask(void *pArg);
static void sCollectUbxNavPvt(const UBX_NAV_PVT_PAYLOAD_t *pkPvt);

void gnssStartTask(void)
{
    DEBUG("gnss: start");

    osMutexCreate(&sEpochMx);
    osSemaphoreCreate(&sNewEpochSem, 0);

    sMsssLastNofix = osTaskGetMsss();
#if (FF_GNSS_PARSER == 1)
    ubxInit();
//    sCollectItow = UINT32_MAX;
#endif

    static OS_TASK_t task;
    static uint8_t stack[FF_GNSS_TASK_STACK];
    osTaskCreate("gns", FF_GNSS_TASK_PRIO, &task, stack, sizeof(stack), sGnssTask,  NULL);
}

//--------------------------------------------------------------------------------------------------

#ifndef __DOXYGEN__ // STFU
static const char skFixTypeStrs[][6] PROGMEM =
{
    { "NOFIX\0" }, { "DR\0" }, { "2D\0" }, { "3D\0" }, { "SF\0" }, { "TIME\0" }
};
#endif

void gnssStatus(char *str, const uint8_t size)
{
    const uint16_t ttff = sEpoch.ttff / 10;
    const uint8_t ttffFrac = sEpoch.ttff - (ttff * 10);
    const uint32_t age = (osTaskGetMsss() - sMsssLastEpoch + 500) / 100;
    const uint16_t ageI = age / 10;
    const uint16_t ageF = age - (ageI * 10);
    snprintf_P(str, size, PSTR("numEpochs=%"PRIu32" fixType=%S ttff=%"PRIu16".%"PRIu16" age=%"PRIu16".%"PRIu16" numSV=%"PRIu8),
        sEpoch.count, skFixTypeStrs[sEpoch.fixType], ttff, ttffFrac, ageI, ageF, (uint8_t)sEpoch.numSv);
    str[size-1] = '\0';
}

#ifndef __DOXYGEN__ // STFU
static const char skTaccStrs[][7] PROGMEM =
{
    { "<100ns\0" }, { "<1ms\0" }, { "<10ms\0" }, { "<100ms\0" }, { "<0.5s\0" }, { "<1s\0" }, { "<10s\0" }, { "oo\0" }
};
#endif

#ifndef __DOXYGEN__ // STFU
static const char skPaccStrs[][6] PROGMEM =
{
    { "<1cm\0" }, { "<10cm\0" }, { "<1m\0" }, { "<2m\0" }, { "<5m\0" }, { "<10m\0" }, { "<100m\0" }, { "oo\0" }
};
#endif

#ifndef __DOXYGEN__ // STFU
static const char skDopStrs[][6] PROGMEM =
{
    { "<0.5\0" }, { "<1.0\0" }, { "<1.5\0" }, { "<2.0\0" }, { "<2.5\0" }, { "<5.0\0" }, { "<10.0\0" }, { "oo\0" }
};
#endif

void gnssStringifyEpoch(const GNSS_EPOCH_t *pkEpoch, char *str, const uint8_t size)
{
    const uint16_t ttff = sEpoch.ttff / 10;
    const uint8_t ttffFrac = sEpoch.ttff - (ttff * 10);
    snprintf_P(str, size, PSTR("#%"PRIu32" (%"PRIu16".%"PRIu16") %S (%c) #SV=%"PRIu8" PDOP=%S "
            "%09.6f/%010.6f/%04"PRIi16" (%S/%S) "
            "%02"PRIu8".%02"PRIu8".%02"PRIu8" (%c) "
            "%02"PRIu8":%02"PRIu8":%02"PRIu8" (%c%c, %S)"
            ),
        pkEpoch->count, ttff, ttffFrac, skFixTypeStrs[pkEpoch->fixType],
        pkEpoch->fixOk ? 'Y' : 'N', (uint8_t)pkEpoch->numSv, skDopStrs[pkEpoch->pDop],
        pkEpoch->lat, pkEpoch->lon, pkEpoch->alt, skPaccStrs[pkEpoch->hAcc], skPaccStrs[pkEpoch->vAcc],
        (uint8_t)pkEpoch->day, (uint8_t)pkEpoch->month, (uint8_t)pkEpoch->year, pkEpoch->dateValid ? 'Y' : 'N',
        (uint8_t)pkEpoch->hour, (uint8_t)pkEpoch->min, (uint8_t)pkEpoch->sec,
        pkEpoch->timeValid ? 'Y' : 'N', pkEpoch->leapValid ? 'Y' : 'N', skTaccStrs[pkEpoch->tAcc]);
}

void gnssStringifyTime(const GNSS_EPOCH_t *pkEpoch, char *str, const uint8_t size)
{
    snprintf_P(str, size, PSTR("%02"PRIu8":%02"PRIu8":%02"PRIu8" (%c%c, %S)"),
        (uint8_t)pkEpoch->hour, (uint8_t)pkEpoch->min, (uint8_t)pkEpoch->sec,
        pkEpoch->timeValid ? 'Y' : 'N', pkEpoch->leapValid ? 'Y' : 'N', skTaccStrs[pkEpoch->tAcc]);
}



//--------------------------------------------------------------------------------------------------

static void sGnssTask(void *pArg)
{
    UNUSED(pArg);

    while (ENDLESS)
    {
        // wait for data to become available on the serial port
        uint8_t n = hwGetRxBufSize(0);

        // delay a bit when there is no data
        // if we don't do this n will often be 1 and we'll be wasting CPU time
        // and we don't need to be very "real-time" here
        if (n == 0)
        {
            osTaskDelay(1);
        }

        // process incoming data
        while (n--)
        {
            const char c = hwReadNextChar();

            // u-blox binary protocol
#if (FF_GNSS_PARSER == 1)

            UBX_MSG_CLS_t msgCls;
            UBX_MSG_ID_t  msgId;
            const UBX_PAYLOADS_t *pkPayload = ubxParse(c, &msgCls, &msgId);
            if (pkPayload == NULL)
            {
                continue;
            }

            //DEBUG("gnss: ubx: %02"PRIx8" %02"PRIx8, msgCls, msgId);
            switch (msgCls)
            {
                case UBX_MSG_CLS_NAV:
                    switch (msgId)
                    {
#  if (FF_UBX_NAV_PVT_USE > 0)
                        case UBX_MSG_ID_NAV_PVT:
                        {
                            const UBX_NAV_PVT_PAYLOAD_t *pkPvt = &pkPayload->navPvt;
                            //const uint32_t iTow = pkPvt->iTOW;
                            //DEBUG("gnss: UBX-NAV-PVT %08"PRIu32" %04"PRIu16"-%02"PRIu8"-%02"PRIu8" %02"PRIu8":%02"PRIu8":%02"PRIu8" %"PRIu8" %"PRIu8" %.5f %.5f %"PRIi32,
                            //    iTow, pkPvt->year, pkPvt->month, pkPvt->day, pkPvt->hour, pkPvt->min, pkPvt->sec,
                            //    pkPvt->fixType, pkPvt->numSV, (float)pkPvt->lat * 1e-7, (float)pkPvt->lon * 1e-7, pkPvt->height / 1000);
                            if (osMutexClaim(&sEpochMx, 1)) // cannot wait too long, need to keep reading on the input
                            {
                                sCollectUbxNavPvt(pkPvt);

                                sEpoch.count++;
                                sMsssLastEpoch = osTaskGetMsss();

                                osMutexRelease(&sEpochMx);

                                osSemaphoreGive(&sNewEpochSem, true);
                            }
                            else
                            {
                                ERROR("gnss: mx");
                            }
                        }   break;
#  endif // (FF_UBX_NAV_PVT_USE > 0)
                        default:
                            break;
                    }
                    break;
                default:
                    break;

            } // switch (msgCls)
#endif // (FF_GNSS_PARSER == 1)

        } // while (n--)

    } // ENDLESS
}

//--------------------------------------------------------------------------------------------------

static void sCollectUbxNavPvt(const UBX_NAV_PVT_PAYLOAD_t *pkPvt)
{
    switch (pkPvt->fixType)
    {
        case UBX_NAV_PVT_FIXTYPE_DRFIX: sEpoch.fixType = GNSS_FIXTYPE_DR;   break;
        case UBX_NAV_PVT_FIXTYPE_2DFIX: sEpoch.fixType = GNSS_FIXTYPE_2D;   break;
        case UBX_NAV_PVT_FIXTYPE_3DFIX: sEpoch.fixType = GNSS_FIXTYPE_3D;   break;
        case UBX_NAV_PVT_FIXTYPE_SFFIX: sEpoch.fixType = GNSS_FIXTYPE_SF;   break;
        case UBX_NAV_PVT_FIXTYPE_TIFIX: sEpoch.fixType = GNSS_FIXTYPE_TIME; break;
        case UBX_NAV_PVT_FIXTYPE_NOFIX:
        default:                        sEpoch.fixType = GNSS_FIXTYPE_NONE; break;
    }
    sEpoch.numSv     = pkPvt->numSV & 0x1f;
    sEpoch.dateValid = UBX_NAV_PVT_VALID_DATE(pkPvt->valid) ? true : false;
    sEpoch.timeValid = UBX_NAV_PVT_VALID_TIME(pkPvt->valid) ? true : false;
    sEpoch.leapValid = UBX_NAV_PVT_VALID_LEAP(pkPvt->valid) ? true : false;
    sEpoch.fixOk     = UBX_NAV_PVT_VALID_LEAP(pkPvt->flags) ? true : false;
    sEpoch.day       = pkPvt->day   & 0x1f;
    sEpoch.month     = pkPvt->month & 0x0f;
    sEpoch.year      = (pkPvt->year - 2000) & 0x7f;
    sEpoch.hour      = pkPvt->hour & 0x1f;
    sEpoch.min       = pkPvt->min  & 0x3f;
    sEpoch.sec       = pkPvt->sec  & 0x3f;
    sEpoch.lat       = (float)pkPvt->lat * 1e-7f;
    sEpoch.lon       = (float)pkPvt->lon * 1e-7f;
    sEpoch.alt       = pkPvt->height / 1000;
    if      (pkPvt->tAcc <      100) { sEpoch.tAcc = GNSS_TACC_100NS; }
    else if (pkPvt->tAcc <     1000) { sEpoch.tAcc = GNSS_TACC_1MS;   }
    else if (pkPvt->tAcc <    10000) { sEpoch.tAcc = GNSS_TACC_10MS;  }
    else if (pkPvt->tAcc <   100000) { sEpoch.tAcc = GNSS_TACC_100MS; }
    else if (pkPvt->tAcc <   500000) { sEpoch.tAcc = GNSS_TACC_500MS; }
    else if (pkPvt->tAcc <  1000000) { sEpoch.tAcc = GNSS_TACC_1S;    }
    else if (pkPvt->tAcc < 10000000) { sEpoch.tAcc = GNSS_TACC_10S;   }
    else                             { sEpoch.tAcc = GNSS_TACC_BIG;   }
    if      (pkPvt->hAcc <       10) { sEpoch.hAcc = GNSS_PACC_1CM;   }
    else if (pkPvt->hAcc <      100) { sEpoch.hAcc = GNSS_PACC_10CM;  }
    else if (pkPvt->hAcc <     1000) { sEpoch.hAcc = GNSS_PACC_1M;    }
    else if (pkPvt->hAcc <     2000) { sEpoch.hAcc = GNSS_PACC_2M;    }
    else if (pkPvt->hAcc <     5000) { sEpoch.hAcc = GNSS_PACC_5M;    }
    else if (pkPvt->hAcc <    10000) { sEpoch.hAcc = GNSS_PACC_10M;   }
    else if (pkPvt->hAcc <   100000) { sEpoch.hAcc = GNSS_PACC_100M;  }
    else                             { sEpoch.hAcc = GNSS_PACC_BIG;   }
    if      (pkPvt->vAcc <       10) { sEpoch.vAcc = GNSS_PACC_1CM;   }
    else if (pkPvt->vAcc <      100) { sEpoch.vAcc = GNSS_PACC_10CM;  }
    else if (pkPvt->vAcc <     1000) { sEpoch.vAcc = GNSS_PACC_1M;    }
    else if (pkPvt->vAcc <     2000) { sEpoch.vAcc = GNSS_PACC_2M;    }
    else if (pkPvt->vAcc <     5000) { sEpoch.vAcc = GNSS_PACC_5M;    }
    else if (pkPvt->vAcc <    10000) { sEpoch.vAcc = GNSS_PACC_10M;   }
    else if (pkPvt->vAcc <   100000) { sEpoch.vAcc = GNSS_PACC_100M;  }
    else                             { sEpoch.vAcc = GNSS_PACC_BIG;   }
    if      (pkPvt->pDOP <       50) { sEpoch.pDop = GNSS_DOP_0_5;    }
    else if (pkPvt->pDOP <      100) { sEpoch.pDop = GNSS_DOP_1_0;    }
    else if (pkPvt->pDOP <      150) { sEpoch.pDop = GNSS_DOP_1_5;    }
    else if (pkPvt->pDOP <      200) { sEpoch.pDop = GNSS_DOP_2_0;    }
    else if (pkPvt->pDOP <      250) { sEpoch.pDop = GNSS_DOP_2_5;    }
    else if (pkPvt->pDOP <      500) { sEpoch.pDop = GNSS_DOP_5_0;    }
    else if (pkPvt->pDOP <     1000) { sEpoch.pDop = GNSS_DOP_10_0;   }
    else                             { sEpoch.pDop = GNSS_DOP_BIG;    }
    const uint32_t msss = osTaskGetMsss();
    sEpoch.ttff = ( (msss - sMsssLastNofix) + 500 ) / 100;
    if ( (GNSS_FIXTYPE_2D <= sEpoch.fixType) && (sEpoch.fixType >= GNSS_FIXTYPE_SF) )
    {
        sMsssLastNofix = msss;
    }
}

//--------------------------------------------------------------------------------------------------

//@}
// eof

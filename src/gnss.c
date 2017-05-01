/*!
    \file
    \brief flipflip's Arduino Uno stuff: GNSS receiver abstraction (see \ref GNSS)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup GNSS
    @{

    \todo use timepulse output to determine age of epoch
    \todo reset TTFF when position has been lost and start counting again
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


static float       sGnssTtff    = 0.0f;    // time to first fix (as reported by the receiver) [s]
static float       sGnssLat     = 0.0f;    // latitude [deg]
static float       sGnssLon     = 0.0f;    // longitude [deg]
static float       sGnssAlt     = 0.0f;    // height above ellipsoid [m]
static uint8_t     sGnssFixType = 0;       // fix type (0 = nofix, 1 = dr, 2 = 2d, 3 = 3d, 4 = 3d+dr)
static bool        sGnssFixOk   = false;   // fix within DOP and accuracy mask?
static uint8_t     sGnssNumSv   = 0;       // number of SVs used in navigation solution
static GNSS_TIME_t sGnssTime    = { .hour = 0, .min = 0, .sec = 0, .acc = 0xffff, .valid = false, .leap = false }; // latest time solution
static OS_MUTEX_t     sGnssTimeMutex;       // access protection for sGnssTime
static OS_SEMAPHORE_t sGnssNewTimeSemaphore; // new time solution available semaphore

void gnssStatus(char *str, const uint8_t size)
{
    snprintf_P(str, size, PSTR("ttff=%.1f llh=%.4f/%.4f/%.0f fix=%"PRIu8"/%c #SV=%"PRIu8" time=%c"),
        sGnssTtff, sGnssLat, sGnssLon, sGnssAlt, sGnssFixType, sGnssFixOk ? 'Y' : 'N', sGnssNumSv,
        sGnssTime.valid ? 'Y' : 'N');
    str[size-1] = '\0';
}

bool gnssGetTime(GNSS_TIME_t *pTime, const int8_t tzOffs, int32_t timeout)
{
    if (timeout >= 0)
    {
        if (osSemaphoreTake(&sGnssNewTimeSemaphore, timeout) == false)
        {
            return false;
        }
    }

    osMutexClaim(&sGnssTimeMutex, 0);
    memcpy(pTime, &sGnssTime, sizeof(*pTime));

    int8_t hour = pTime->hour;
    hour += tzOffs;
    if (hour < 0)
    {
        hour += 24;
    }
    else if (hour >= 24)
    {
        hour -= 24;
    }
    pTime->hour = (uint8_t)hour;;

    osMutexRelease(&sGnssTimeMutex);

    return true;
}

static void sGnssTask(void *pArg)
{
    UNUSED(pArg);

    while (ENDLESS)
    {
        // wait for data to become available on the serial port
        uint8_t n = hwGetRxBufSize(0);

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
                            sGnssLat     = (float)pkPvt->lat * 1e-7f;
                            sGnssLon     = (float)pkPvt->lon * 1e-7f;
                            sGnssAlt     = (float)pkPvt->height * 1e-3f;
                            sGnssFixType = pkPvt->fixType;
                            sGnssNumSv   = pkPvt->numSV;
                            sGnssFixOk   = UBX_NAV_PVT_FLAGS_FIXOK(pkPvt->flags) ? true : false;

                            // make new time available
                            if (osMutexClaim(&sGnssTimeMutex, 1)) // cannot wait too long, need to keep reading on the input
                            {
                                //DEBUG("gnss: new time");
                                sGnssTime.hour  = pkPvt->hour;
                                sGnssTime.min   = pkPvt->min;
                                sGnssTime.sec   = pkPvt->sec;
                                sGnssTime.acc   = (pkPvt->tAcc > (uint32_t)4294) ? 4295 : (uint16_t)(pkPvt->tAcc / (uint32_t)1000000);
                                sGnssTime.valid = UBX_NAV_PVT_VALID_TIME(pkPvt->valid) ? true : false;
                                sGnssTime.leap  = UBX_NAV_PVT_VALID_LEAP(pkPvt->valid) ? true : false;
                                osMutexRelease(&sGnssTimeMutex);
                                osSemaphoreGive(&sGnssNewTimeSemaphore, true);
                            }
                            else
                            {
                                ERROR("gnss: mx");
                            }
                            //DEBUG("gnss: %04"PRIu16"-%02"PRIu8"-%02"PRIu8" %02"PRIu8":%02"PRIu8":%02"PRIu8" %"PRIu8" %"PRIu8" %.5f %.5f %"PRIi32,
                            //    pkPvt->year, pkPvt->month, pkPvt->day, pkPvt->hour, pkPvt->min, pkPvt->sec,
                            //    pkPvt->fixType, pkPvt->numSV, (float)pkPvt->lat * 1e-7, (float)pkPvt->lon * 1e-7, pkPvt->height / 1000);
                        }   break;
#  endif // (FF_UBX_NAV_PVT_USE > 0)
#  if (FF_UBX_NAV_STATUS_USE > 0)
                        case UBX_MSG_ID_NAV_STATUS:
                        {
                            const UBX_NAV_STATUS_PAYLOAD_t *pkStatus = &pkPayload->navStatus;
                            sGnssTtff = (float)pkStatus->ttff * 1e-3f;
                            //DEBUG("gnss: %.3f %.3f", (float)pkStatus->ttff * 1e-3, (float)pkStatus->msss * 1e-3);
                        }   break;
#  endif // (FF_UBX_NAV_STATUS_USE > 0)
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

void gnssStartTask(void)
{
    DEBUG("gnss: start");
    osMutexCreate(&sGnssTimeMutex);
    osSemaphoreCreate(&sGnssNewTimeSemaphore, 0);
    static OS_TASK_t task;
    static uint8_t stack[FF_GNSS_TASK_STACK];
    osTaskCreate("gns", FF_GNSS_TASK_PRIO, &task, stack, sizeof(stack), sGnssTask,  NULL);
}



//@}
// eof

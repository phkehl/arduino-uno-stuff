/*!
    \file
    \brief flipflip's Arduino Uno stuff: sixth example application (see \ref EXAMPLES_EX6)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX6
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ubx.h"           // ff: u-blox binary protocol
#include "gnss.h"          // ff: GNSS receiver abstraction

#include "ex6.h"


//#define RAW_UBX


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex6: init, sizeof(GNSS_EPOCH_t)=%"PRIu8,
        (uint8_t)sizeof(GNSS_EPOCH_t));

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

#ifdef RAW_UBX
    ubxInit();
#endif
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[200];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

#ifndef RAW_UBX
    gnssStartTask();
#endif
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    //static UBX_PAYLOADS_t sPayload;

    while (ENDLESS)
    {
#ifdef RAW_UBX
        uint8_t n = hwGetRxBufSize(0);
        while (n--)
        {
            const char c = hwReadNextChar();
            UBX_MSG_CLS_t msgCls;
            UBX_MSG_ID_t  msgId;
            const UBX_PAYLOADS_t *pkPayload = ubxParse(c, &msgCls, &msgId);
            if (pkPayload != NULL)
            {
                DEBUG("ubx %02"PRIx8" %02"PRIx8 " %"PRIu32,
                    msgCls, msgId,
                    // UBX-NAV-*.iTOW field
                    msgCls == UBX_MSG_CLS_NAV ? *((const uint32_t *)((const uint8_t *)pkPayload)) : 0);
            }
        }
#else
        static GNSS_EPOCH_t epoch;
        if (gnssGetEpoch(&epoch, 5000))
        {
            static char str[120];
            gnssStringifyEpoch(&epoch, str, sizeof(str));
            PRINT("epoch %s", str);
        }
        else
        {
            WARNING("no epoch :-(");
        }
#endif
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    ubxStatus(str, size);
    hwTxFlush();
    PRINT("mon: ubx: %s", str);
    gnssStatus(str, size);
    hwTxFlush();
    PRINT("mon: gnss: %s", str);
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof

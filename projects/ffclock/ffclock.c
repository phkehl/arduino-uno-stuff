/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFCLOCK
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ubx.h"           // ff: u-blox binary protocol
#include "gnss.h"          // ff: GNSS receiver abstraction

#include "ffclock.h"


/* ***** configuration ******************************************************* */

// clock stepper motor signal pins
#define CLKMOT_PIN_I1 _PB1
#define CLKMOT_PIN_I2 _PB3
#define CLKMOT_PIN_I3 _PB2
#define CLKMOT_PIN_I4 _PB0



/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ffclock: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    // initialise clock motor pins
    PIN_OUTPUT(CLKMOT_PIN_I1);
    PIN_OUTPUT(CLKMOT_PIN_I2);
    PIN_OUTPUT(CLKMOT_PIN_I3);
    PIN_OUTPUT(CLKMOT_PIN_I4);
    PIN_LOW(CLKMOT_PIN_I1);
    PIN_LOW(CLKMOT_PIN_I2);
    PIN_LOW(CLKMOT_PIN_I3);
    PIN_LOW(CLKMOT_PIN_I4);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

    gnssStartTask();
}


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    // keep running...
    while (ENDLESS)
    {
        GNSS_TIME_t time;
        if (gnssGetTime(&time, 0, 5000))
        {
            PRINT("time: %02"PRIu8":%02"PRIu8":%02"PRIu8" acc=%"PRIu16"ms valid=%c leap=%c",
                time.hour, time.min, time.sec, time.acc,
                time.valid ? 'Y' : 'N', time.leap ? 'Y' : 'N');
        }
        else
        {
            WARNING("no time :-(");
        }
    } // ENDLESS
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

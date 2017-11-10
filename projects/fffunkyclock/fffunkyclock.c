/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFFUNKYCLOCK)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFFUNKYCLOCK
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities
#include <avr/eeprom.h>    // avr-libc: EEPROM handling

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ubx.h"           // ff: u-blox binary protocol
#include "gnss.h"          // ff: GNSS receiver abstraction
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects

#include "fffunkyclock.h"


/* ***** configuration ******************************************************* */


/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    ledfxSetBrightness(100);

    uint8_t hue0 = 0;

    while (ENDLESS)
    {
        DEBUG("hoihoi.. %"PRIu8" %"PRIu16"/%"PRIu16" %"PRIu16,
            hue0, FF_LEDFX_NUM_LED, ledfxNumLeds(), ledfxGetFrameBufferSize());

        uint8_t hue = hue0;
        for (uint16_t ix = 0; ix < FF_LEDFX_NUM_LED; ix++)
        {
            ledfxSetIxHSV(ix, hue, 255, 255);
            hue += 16;
        }

        hue0 += 4;

        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

        osTaskDelay(50);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("clock..."));
}


/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("fffunkyclock: init");

    ws2801Init();

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);

    // also start the GNSS task (handles reading from the receiver and collects epochs)
    //gnssStartTask();
}

//------------------------------------------------------------------------------

//@}
// eof

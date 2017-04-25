/*!
    \file
    \brief flipflip's Arduino Uno stuff: third example application (see \ref EXAMPLES_EX3)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup EXAMPLES_EX3
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "ws2812.h"        // ff: WS2812 LED driver
#include "ledfx.h"         // ff: LED effects

#include "ex3.h"

/* ***** application init **************************************************** */

#define LED_PIN 9
#define STRIP_PIN 7

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex3: init");

    // a LED
    PIN_OUTPUT(LED_PIN);
    PIN_LOW(LED_PIN);

    // WS2812 LED strip
    PIN_OUTPUT(STRIP_PIN);
    PIN_LOW(STRIP_PIN);

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    // set global LED brightness
    ledfxSetBrightness(100);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application functions *********************************************** */

#define MA_PER_LED 40
#define PSU_MAX_MA 1000


/* ***** application task **************************************************** */

static uint16_t sAppCurrent;
static uint32_t sAppTick;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // keep running...
    while (ENDLESS)
    {
        osTaskDelay(50);

        // set the LED
        PIN_HIGH(LED_PIN);

        // randomly set 5 LEDs on the strip, initialise every ~5 seconds
        const bool init = (sAppTick % 1000) == 0;
        if (init)
        {
            ledfxClear(0, 0);
        }
        ledfxNoiseRandomDistinct(init, 0, 0, 5);

        // limit current
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);

        // send data to the LED
        ws2812Send(STRIP_PIN, ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

        sAppTick++;
        // set the LED
        PIN_LOW(LED_PIN);

    } // ENDLESS
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("sAppCurrent=%"PRIu16), sAppCurrent);
}


//@}
// eof

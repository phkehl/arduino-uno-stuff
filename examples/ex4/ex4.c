/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourth example application (see \ref EXAMPLES_EX4)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX4
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
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects

#include "ex4.h"


/* ***** application init **************************************************** */

#define LED_PIN _D9
#define LED2_PIN _D8

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex4: init");

    ws2801Init();

    // another LED
    PIN_OUTPUT(LED2_PIN);
    PIN_LOW(LED2_PIN);

    // a LED
    PIN_OUTPUT(LED_PIN);
    PIN_LOW(LED_PIN);

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}


// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application functions *********************************************** */


/* ***** application task **************************************************** */

static uint32_t sAppCnt = 0;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    static uint8_t sHue = 0;
    ledfxSetBrightness(50);

    // keep running...
    while (ENDLESS)
    {
        osTaskDelay(20);
        sAppCnt++;

        // toggle the LED
        PIN_TOGGLE(LED_PIN);

        // sweep hue value
        ledfxSetIxHSV(0, sHue, 255, 255);
        ledfxSetIxHSV(1, sHue, 255, 255);
        sHue++;

        // write data to the LEDs
        PIN_HIGH(LED2_PIN);
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

        PIN_LOW(LED2_PIN);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("sAppCnt=%"PRIu32), sAppCnt);
}


//@}
// eof

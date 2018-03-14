/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourth example application (see \ref EXAMPLES_EX13)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX13
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "sk9822.h"        // ff: SK9822 LED driver
#include "ledfx.h"         // ff: LED effects

#include "ex13.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex4: init");

    sk9822Init();

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
    //ledfxSetBrightness(50);
    sk9822SetBrightness(1);


    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;

        // sweep hue value
        ledfxFillHSV(0, 0, sHue, 255, 20);
        sHue += 7;

        for (uint8_t brightness = 1; brightness < 32; brightness++)
        {
            sk9822SetBrightness(brightness);
            sk9822Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
            osTaskDelay(50);
        }
        for (uint8_t brightness = 32; brightness > 0; brightness--)
        {
            sk9822SetBrightness(brightness);
            sk9822Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
            osTaskDelay(50);
        }
        osTaskDelay(250);
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

/*!
    \file
    \brief flipflip's Arduino Uno stuff: eighth example application (see \ref EXAMPLES_EX08)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX08
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ledfx.h"         // ff: LED effects
#include "alimatrix.h"     // ff: Aliexpress LED matrix driver

#include "ex08.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex8: init");

    alimatrixInit();

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


/* ***** application task **************************************************** */

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    alimatrixStart();

    ledfxClear(0, 0);

    static uint32_t msss;
    msss = osTaskGetTicks();
    static uint16_t n;
    static float sF;
    static uint8_t sU;

    while (ENDLESS)
    {
        DEBUG("rgbdemo: all");
        ledfxClear(0, 0);
        ledfxFillRGB( 0,  7, 255,   0,   0);
        ledfxFillRGB( 8, 15,   0, 255,   0);
        ledfxFillRGB(16, 23,   0,   0, 255);
        ledfxFillRGB(24, 31, 255, 255,   0);
        ledfxFillRGB(32, 39, 255,   0, 255);
        ledfxFillRGB(40, 47,   0, 255, 255);
        ledfxFillRGB(48, 55, 255, 255, 255);
        ledfxFillRGB(56, 63,   0,   0,   0);
        alimatrixUpdate(ledfxGetFrameBuffer());
        osTaskDelay(5000);

        DEBUG("rgbdemo: shades");
        ledfxClear(0, 0);
        for (uint8_t ix = 0; ix < 8; ix++)
        {
            const uint8_t val = 1 + (((uint16_t)ix * 254) / 7);
            ledfxSetMatrixRGB(ix, 0, val, 0, 0);
            ledfxSetMatrixRGB(ix, 1, 0, val, 0);
            ledfxSetMatrixRGB(ix, 2, 0, 0, val);
            ledfxSetMatrixRGB(ix, 3, val, val, 0);
            ledfxSetMatrixRGB(ix, 4, val, 0, val);
            ledfxSetMatrixRGB(ix, 5, 0, val, val);
            ledfxSetMatrixRGB(ix, 6, val, val, val);
        }
        alimatrixUpdate(ledfxGetFrameBuffer());
        osTaskDelay(10000);

        DEBUG("rgbdemo: red");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, val, 0, 0);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: green");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, 0, val, 0);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: blue");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, 0, 0, val);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: red+green");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, val, val, 0);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: red+blue");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, val, 0, val);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: green+blue");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, 0, val, val);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        DEBUG("rgbdemo: red+green+blue");
        for (uint8_t val = 1; val < 250; val += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, val, val, val);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }

        DEBUG("huedemo");
        for (uint8_t hue = 0; hue < 255; hue += 5)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillHSV(0, 0, hue, 255, 255);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }

        DEBUG("hueflow");
        n = 400;
        ledfxConcentricHueFlow(true, 1, &sU);
        while (n--)
        {
            osTaskDelayUntil(&msss, 50);
            ledfxConcentricHueFlow(false, 1, &sU);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }

        DEBUG("rainbow");
        n = 200;
        ledfxRainbow(true, 0, 0, &sU);
        while (n--)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxRainbow(false, 0, 0, &sU);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }

        DEBUG("plasma");
        n = 200;
        ledfxPlasma(true, &sF);
        while (n--)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxPlasma(false, &sF);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof

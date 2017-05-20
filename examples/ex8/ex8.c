/*!
    \file
    \brief flipflip's Arduino Uno stuff: eighth example application (see \ref EXAMPLES_EX8)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX8
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

#include "ex8.h"


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
        DEBUG("rgbdemo");
        for (uint8_t val = 5; val < 255; val += 10)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, val, 0, 0);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        for (uint8_t val = 5; val < 255; val += 10)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, 0, val, 0);
            alimatrixUpdate(ledfxGetFrameBuffer());
        }
        for (uint8_t val = 5; val < 255; val += 10)
        {
            osTaskDelayUntil(&msss, 100);
            ledfxFillRGB(0, 0, 0, 0, val);
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

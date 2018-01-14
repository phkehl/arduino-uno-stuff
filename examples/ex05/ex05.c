/*!
    \file
    \brief flipflip's Arduino Uno stuff: fifth example application (see \ref EXAMPLES_EX05)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX05
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "sys.h"           // ff: system task
#include "fxloop.h"        // ff: effects loops

#include "ex05.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex5: init");

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

static uint16_t sFxloopFunc1(const uint16_t frame)
{
    if (frame == 0)
    {
        DEBUG("sFxloopFunc1() init");
    }
    else
    {
        DEBUG("sFxloopFunc1() frame %"PRIu16, frame);

        if ((frame % 5) == 0)
        {
            // waste CPU time so that we miss a frame
            uint32_t x = 1000000;
            while (x--)
            {
            }
        }
    }
    return frame;
}

static uint16_t sFxloopFunc2(const uint16_t frame)
{
    if (frame == 0)
    {
        DEBUG("sFxloopFunc2() init");
    }
    else
    {
        DEBUG("sFxloopFunc2() frame %"PRIu16, frame);
    }
    return frame;
}

static uint16_t sFxloopFunc3(const uint16_t frame)
{
    if (frame == 0)
    {
        DEBUG("sFxloopFunc3() init");
    }
    else
    {
        DEBUG("sFxloopFunc3() frame %"PRIu16, frame);
    }
    return frame;
}


/* ***** application task **************************************************** */

static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
    FXLOOP_INFO("func1func1func1", sFxloopFunc1, 250, 250, 3000),
    FXLOOP_INFO("func2",           sFxloopFunc2, 300, 500, 3000),
    FXLOOP_INFO("func3",           sFxloopFunc3, 250, 750, 3000),
};

static uint32_t sAppCnt;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;

        // run one iteration of the effect
        const bool forceNext = false;
        const uint16_t res = fxloopRun(forceNext);
        DEBUG("res=%"PRIu16, res);

        // do other stuff.. such as wasting some CPU time
        uint16_t n = 10000;
        while (n--)
        {
        }

        // delay until it's time to run the next iteration of the effect
        const bool fxWillChange = fxloopWait(50); // play at 50% speed

        if (fxWillChange)
        {
            DEBUG("prepare for next effect...");
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    const int n = snprintf_P(str, size, PSTR("cnt=%"PRIu32" "), sAppCnt);
    fxloopStatus(&str[n], size - n);
}


//@}
// eof

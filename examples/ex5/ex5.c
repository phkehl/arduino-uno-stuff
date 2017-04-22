/*!
    \file
    \brief flipflip's Arduino Uno stuff: fifth example application (see \ref EXAMPLES_EX5)

    - Copyright (c) 2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup EXAMPLES_EX5
    @{
*/

#include <string.h>        // libc: string operations

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "sys.h"           // ff: system task
#include "fxloop.h"        // ff: effects loops

#include "ex5.h"


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
    static U1 stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application functions *********************************************** */

static L sFxloopFunc1(const L init)
{
    static U1 n;
    if (init)
    {
        n = 0;
        DEBUG("sFxloopFunc1() init");
    }
    else
    {
        n++;
        DEBUG("sFxloopFunc1() run %"F_U1, n);

        if ((n % 5) == 0)
        {
            // waste CPU time so that we miss a frame
            U4 x = 1000000;
            while (x--)
            {
            }
        }
    }
    return true;
}

static L sFxloopFunc2(const L init)
{
    if (init)
    {
        DEBUG("sFxloopFunc2() init");
    }
    else
    {
        DEBUG("sFxloopFunc2() run");
    }
    return true;
}

static L sFxloopFunc3(const L init)
{
    if (init)
    {
        DEBUG("sFxloopFunc3() init");
    }
    else
    {
        DEBUG("sFxloopFunc3() run");
    }
    return true;
}


/* ***** application task **************************************************** */

static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
    { .fxName = "func1", .fxFunc = sFxloopFunc1, .fxPeriod = 250, .fxDuration = 3000 },
    { .fxName = "func2", .fxFunc = sFxloopFunc2, .fxPeriod = 250, .fxDuration = 4000 },
    { .fxName = "func3", .fxFunc = sFxloopFunc3, .fxPeriod = 250, .fxDuration = 5000 },
};

static U4 sAppCnt;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    // keep running...
    while (ENDLESS)
    {
        sAppCnt++;

        // run one iteration of the effect
        fxloopRun(false);

        // do other stuff.. such as wasting some CPU time
        U2 n = 10000;
        while (n--)
        {
        }

        // delay until it's time to run the next iteration of the effect
        fxloopWait();
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    const int n = snprintf_P(str, size, PSTR("cnt=%"F_U4" "), sAppCnt);
    fxloopStatus(&str[n], size - n);
}


//@}
// eof

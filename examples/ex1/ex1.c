/*!
    \file
    \brief flipflip's Arduino Uno stuff: first example application (see \ref EXAMPLES_EX1)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup EXAMPLES_EX1
    @{
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "ex1.h"


/* ***** application init **************************************************** */

#define LED_PIN 9
#define LED2_PIN 8

// forward declaration
static void sAppStatus(char *str, const size_t size);

// initialise the user application
void appInit(void)
{
    DEBUG("ex1: init");

    // LED
    PIN_OUTPUT(LED_PIN);
    PIN_LOW(LED_PIN);

    // another LED
    PIN_OUTPUT(LED2_PIN);
    PIN_LOW(LED2_PIN);

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// forward declaration
static void sAppTask(void *pArg);

// starts the user application task
void appCreateTask(void)
{
    static U1 stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}



/* ***** application task **************************************************** */

static U4 sAppCnt = 0;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // keep running...
    while (ENDLESS)
    {
        // toggle the LED
        PIN_TOGGLE(LED_PIN);

        // delay a bit
        osTaskDelay(123);
        DEBUG("foo");
        osTaskDelay(123);
        DEBUG("bar");
        osTaskDelay(123);
        DEBUG("baz");

        // count up
        sAppCnt++;

        // waste some CPU
        PIN_HIGH(LED2_PIN);
        U4 foo = 0;
        for (U ix = 0; ix < 12345; ix++)
        {
            foo += ix;
        }
        PIN_LOW(LED2_PIN);
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("sAppCnt=%"F_U4), sAppCnt);
}


//@}
// eof
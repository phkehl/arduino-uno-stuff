/*!
    \file
    \brief flipflip's Arduino Uno stuff: Mains frequency measurement (see \ref PROJECTS_MAINSFREQ)

    - Copyright (c) 2022 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_MAINSFREQ
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "mainsfreq.h"

static OS_SEMAPHORE_t sMeasureNow;

#define NUM_INTS 100 // Trigger timepulse at 1 Hz for nominal 50 Hz mains

// External interrupt 0, triggered on all zero crossings (~100 Hz)
ISR(INT0_vect)
{
    static volatile int svPulseCount;
    osIsrEnter();
    svPulseCount++;
    if (svPulseCount == NUM_INTS)
    {
        PIN_HIGH(_PD3);
        svPulseCount = 0;
        osSemaphoreGive(&sMeasureNow, true);
    }
    osIsrLeave();
}

// Application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    uint32_t prev = osTaskGetTicks();

    while (ENDLESS)
    {
        sysAssertSwWatchdog();
        if (osSemaphoreTake(&sMeasureNow, 5000))
        {
            const uint32_t now = osTaskGetTicks();
            const double dt = (double)(now - prev) * 1e-3;
            const double freq = /*(1.0/2.0) / (dt / (double)NUM_INTS) =*/ ((double)NUM_INTS/2.0) / dt;
            prev = now;
            PRINT("freq: %.1f Hz", freq); // approx!

            osTaskDelay(200);
            PIN_LOW(_PD3);
        }
        else
        {
            WARNING("no measurements?!");
        }
    }
}

// Initialise the user application
void appInit(void)
{
    DEBUG("mainsfreq: init");

    // INT0
    PIN_INPUT(_PD2);
    PIN_PULLUP_ON(_PD2);
    EICRA = BIT(ISC01) | BIT(ISC00);   // INT0 on raising edge
    EIMSK = BIT(INT0);                 // Enable INT0
    EIFR  = BIT(INTF0);                // Clear interrupt

    PIN_OUTPUT(_PD3);
    PIN_LOW(_PD3);

    osSemaphoreCreate(&sMeasureNow, 0);
}

// Starts the user application tasks
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: Maschki application (see \ref PROJECTS_MASCHKI)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_MASCHKI
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ws2801.h"        // ff: WS2811 LED driver
#include "ledfx.h"         // ff: LED effects

#include "maschki.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

static OS_SEMAPHORE_t sIsrActivitySem;
static OS_SEMAPHORE_t sIsrRangeSem;

// initialise the user application
void appInit(void)
{
    DEBUG("maschki: init");

    // RGB LEDs
    ws2801Init();

    // PIR sensor
    PIN_INPUT(_D3);
    PIN_PULLUP_OFF(_PD3);
    osSemaphoreCreate(&sIsrActivitySem, 0);
    EICRA |= BIT(ISC11) | BIT(ISC10);   // raising edge triggers INT1
    EIMSK |= BIT(INT1);                 // enable INT1 interrupt
    EIFR  |= BIT(INTF1);                // clear interrupt

    // range sensor
    PIN_OUTPUT(_D4);
    PIN_LOW(_D4);
    PIN_INPUT(_D2);
    PIN_PULLUP_OFF(_D2);
    osSemaphoreCreate(&sIsrRangeSem, 0);
    EICRA |= BIT(ISC00);   // any logical change triggers INT0
    //EIMSK |= BIT(INT0);    // enable INT0
    EIFR  |= BIT(INTF0);   // clear interrupt

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

// PIR sensor activity
ISR(INT1_vect)
{
    osIsrEnter();
    osSemaphoreGive(&sIsrActivitySem, true);
    osIsrLeave();
}

static volatile uint16_t svRangeRaw;

// range measurement
ISR(INT0_vect)
{
    osIsrEnter();

    // raising edge: start measurement
    if (svRangeRaw == 0)
    {
        hwTic(0);
        svRangeRaw++;
    }
    // falling edge: stop measurement
    else
    {
        svRangeRaw = hwToc(0);
        osSemaphoreGive(&sIsrRangeSem, true);
        CLRBITS(EIMSK, BIT(INT0));
    }

    osIsrLeave();
}


/* ***** application task **************************************************** */


// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    //CS_ENTER; while (1) {}; CS_LEAVE;

    static uint8_t sHue = 0;
    ledfxSetBrightness(50);

    static uint8_t rangeCnt;
    while (ENDLESS)
    {
        //osTaskDelay(50);

        if (osSemaphoreTake(&sIsrActivitySem, 50))
        {
            DEBUG("PIR");
        }

        rangeCnt++;
        if (rangeCnt > 10)
        {
            rangeCnt = 0;
            //DEBUG("range");

            // trigger range measurement
            PIN_LOW(_D4);
            PIN_HIGH(_D4);
            hwDelay(10);
            PIN_LOW(_D4);

            // arm interrupt to measure time
            svRangeRaw = 0;
            SETBITS(EIMSK, BIT(INT0));

            // wait for result
            if (osSemaphoreTake(&sIsrRangeSem, 50))
            {
                DEBUG("range=%"PRIu16, svRangeRaw);
            }
            // timeout
            else
            {
                WARNING("range to %"PRIu16, svRangeRaw);
                CLRBITS(EIMSK, BIT(INT0));
            }


            //osTaskDelay(1);
            //hwTic(0);
            //osTaskDelay(2);
            //DEBUG("delay=%"PRIu16, hwToc(0));
        }

        // sweep hue value
        ledfxSetIxHSV(0, sHue,       255, 255);
        ledfxSetIxHSV(1, sHue + 128, 255, 255);
        sHue++;

        // write data to the LEDs
        ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("maschki status"));
}


//@}
// eof

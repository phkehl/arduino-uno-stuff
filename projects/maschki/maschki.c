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

    // PIR (passive infrared) sensor
    PIN_INPUT(_D3);                          // sensor out to pin D3 (= INT1)
    PIN_PULLUP_OFF(_PD3);                    // sensor will drive signal high on activity
    EICRA |= BIT(ISC11) | BIT(ISC10);        // raising edge triggers INT1
    EIMSK |= BIT(INT1);                      // enable INT1 interrupt
    EIFR  |= BIT(INTF1);                     // clear interrupt
    osSemaphoreCreate(&sIsrActivitySem, 0);  // ISR will give this semaphore to signal activity

    // ultrasonic range sensor
    PIN_OUTPUT(_D4);                         // D4 pin signal to trigger range measurement
    PIN_LOW(_D4);
    PIN_INPUT(_D2);                          // echo response from sensor
    PIN_PULLUP_OFF(_D2);                     // sensor will drive high
    EICRA |= BIT(ISC00);                     // any logical change triggers INT0
    EIFR  |= BIT(INTF0);                     // clear interrupt
    osSemaphoreCreate(&sIsrRangeSem, 0);     // ISR will give this semaphore to signal measurement done

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
    // signal waiting task (binary semaphore, i.e. don't increment beyond 1)
    osSemaphoreGive(&sIsrActivitySem, true);
    osIsrLeave();
}

// latest range measurement
static volatile uint16_t svRangeRaw;

// start a range measurement
static void sTriggerRangeMeas(void)
{
    CLRBITS(EIMSK, BIT(INT0)); // disable interrupt

    // trigger sensor to send the pulse
    PIN_LOW(_D4);
    PIN_HIGH(_D4);
    hwDelay(10);
    PIN_LOW(_D4);

    // arm interrupt to measure time of flight
    svRangeRaw = 0;
    EIFR  |= BIT(INTF0);       // clear interrupt
    SETBITS(EIMSK, BIT(INT0)); // enable interrupt
}

// return range in [cm], clipped at 2.55m
static uint8_t sGetRange(void)
{
    const uint32_t c = (uint16_t)340 /* m/s */ * 100 /* cm/m */ / 1000 /* ms/s */; // = 34 cm/ms
    //const uint16_t dt = (svRangeRaw + 500) / 1000; // ms
    //const uint16_t d = c * dt / 2; // distance = 1/2 * dt * c
    const uint32_t d = c * svRangeRaw / 2 / 1000; // distance = 1/2 * dt * c
    if (d > 255)
    {
        return 255;
    }
    else
    {
        return d;
    }
}

// time of flight measurement
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

static uint8_t sDoRangeMeas(void)
{
    sTriggerRangeMeas();

    int32_t timeout = 16; // 7.5ms = 2.55m (one way)

    // wait for result
    if (osSemaphoreTake(&sIsrRangeSem, timeout))
    {
        //DEBUG("range=%"PRIu16"=%"PRIu8, svRangeRaw, sGetRange());
        return sGetRange();
    }
    // timeout
    else
    {
        CLRBITS(EIMSK, BIT(INT0)); // disable interrupt
        return 0;
    }
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
            DEBUG("range %"PRIu8, sDoRangeMeas());
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

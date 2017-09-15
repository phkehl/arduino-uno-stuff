/*!
    \file
    \brief flipflip's Arduino Uno stuff: Maschki application (see \ref PROJECTS_MASCHKI)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_MASCHKI
    @{
*/

#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions

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

// ------------------------------------------------------------------------------

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

static float   sRanges[10];
static uint8_t sRangesIx;
static float   sLastFiltered;

static struct
{
    float mean;
    float filt;
    float std;
    float delta;
} sRange;

static void sUpdateStats(const float range)
{
    sRanges[sRangesIx] = range;
    sRangesIx++;
    sRangesIx %= NUMOF(sRanges);

    const float n = (float)NUMOF(sRanges);

    // running average
    float sum = 0.0;
    for (uint8_t ix = 0; ix < NUMOF(sRanges); ix++)
    {
        sum += sRanges[ix];
    }
    const float mean = sum / n;

    // standard deviation
    float sum2 = 0.0;
    float dsum = 0.0;
    for (uint8_t ix = 0; ix < NUMOF(sRanges); ix++)
    {
        const float delta = (range - mean);
        sum2 += delta * delta;
        dsum += delta;
    }
    float std = sqrtf( sum2 / (n - 1.0) );

    float delta = dsum / n;

    // exponential filter
    const float weight = 0.3;
    const float filt = (weight * range) + ((1.0 - weight) * sLastFiltered);
    sLastFiltered = filt;

    DEBUG(">range.dat %6.1f %6.1f %6.1f %6.1f %6.1f",
        range, mean, std, filt, delta);

    sRange.mean  = mean;
    sRange.std   = std;
    sRange.filt  = filt;
    sRange.delta = delta;
}

static void sDoRangeMeas(void)
{
    sTriggerRangeMeas();

    int32_t timeout = 16; // 7.5ms = 2.55m (one way)

    // wait for result
    if (osSemaphoreTake(&sIsrRangeSem, timeout))
    {
        const float c = 340.0 * 100 / 1000 / 1000; // [cm/us]
        const float d = c * (float)svRangeRaw / 2; // [cm]
        sUpdateStats(d);
    }
    // timeout
    else
    {
        CLRBITS(EIMSK, BIT(INT0)); // disable interrupt
    }
}


/* ***** application task **************************************************** */

static uint16_t sAliveTimeout;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    //CS_ENTER; while (1) {}; CS_LEAVE;

    static uint8_t sHue = 0;
    static uint8_t sTick = 0;
    ledfxSetBrightness(50);

    const uint32_t period = 101; // 101 199 251 293 307 331 499
    static uint32_t msss;
    msss = (osTaskGetTicks() / period + 1) * period;
    while (ENDLESS)
    {
        // wait for next tick
        if (!osTaskDelayUntil(&msss, period))
        {
            const uint32_t now = osTaskGetTicks();
            WARNING("late!");
            msss = (now / period) * period;
        }
        sTick++;

        // detect movement from PIR sensor
        if (osSemaphoreTake(&sIsrActivitySem, -1))
        {
            NOTICE("I see you!");
            sAliveTimeout += 60000 / period;
        }

        // no one there...
        if (sAliveTimeout == 0)
        {
            const uint32_t rnd = hwMathGetRandom();
            const uint8_t val1 = ( rnd       & 0x0f) + 1;
            const uint8_t val2 = ((rnd >> 4) & 0x0f) + 1;
            ledfxSetIxHSV(0, sHue, 255, val1);
            ledfxSetIxHSV(1, sHue, 255, val2);
            ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
            if ((sTick % 10) == 0)
            {
                sHue++;
            }
        }

        // someone's there..
        else
        {
            sDoRangeMeas();

            float val = 255.0 - ( 4 * MIN(sRange.mean, 50.0) );
            ledfxSetIxHSV(0, 0, 255, val);
            ledfxSetIxHSV(1, 0, 255, val);
            ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

            sAliveTimeout--;
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size,
        PSTR("alive=%"PRIu16), sAliveTimeout);
}


//@}
// eof

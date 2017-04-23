/*!
    \file
    \brief flipflip's Arduino Uno stuff: "ffchräis" LED rings application (see \ref PROJECTS_FFCHREIS)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup PROJECTS_FFCHREIS
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "ws2812.h"        // ff: WS2812 LED driver
#include "ledfx.h"         // ff: LED effects
#include "fxloop.h"        // ff: effects loops

#include "ffchreis.h"

// TODO: https://github.com/PaulStoffregen/CapacitiveSensor/blob/master/CapacitiveSensor.cpp


/* ***** application init **************************************************** */

// debug LED
#define LED_PIN A5

// LED lit while data is written to the LED strip
#define FLUSH_LED_PIN A4

// the WS2811 LED strip data pin
#define STRIP_PIN A2

// capacitive touch interface
#define TOUCH_SEND_PIN PD2 // Arduino pin 2, PCINT18
#define TOUCH_RECV_PIN PD3 // Arduino pin 3, PCINT19



// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);
static void sLedFlush(void);

// initialise the user application
void appInit(void)
{
    DEBUG("ffchreis: init");

    // WS2812 LED strip
    PIN_OUTPUT(STRIP_PIN);
    PIN_LOW(STRIP_PIN);

    // another LED
    PIN_OUTPUT(FLUSH_LED_PIN);
    PIN_LOW(FLUSH_LED_PIN);

    // a LED
    PIN_OUTPUT(LED_PIN);
    PIN_LOW(LED_PIN);

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    // select A5 for analog input
    // (connect a pot to A5 for brightness setting, or connect it to VCC for maximum brightness)
    //hwAdcInit(HW_ADC_A5, false);

    // clear all
    ledfxClear(0, 0);
    sLedFlush();
}

// starts the user application task
void appCreateTask(void)
{
    static U1 stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application functions *********************************************** */

#define MA_PER_LED 40
#define PSU_MAX_MA 1000

// 5 concentric rings of WS2812 LEDs
#define N_RING_1 1
#define N_RING_2 8
#define N_RING_3 12
#define N_RING_4 16
#define N_RING_5 24

#define RING_1_IX0 ( 0 )
#define RING_1_IX1 ( (N_RING_1) - 1 )

#define RING_2_IX0 ( (N_RING_1) )
#define RING_2_IX1 ( (N_RING_1) + (N_RING_2) - 1)

#define RING_3_IX0 ( (N_RING_1) + (N_RING_2) )
#define RING_3_IX1 ( (N_RING_1) + (N_RING_2) + (N_RING_3) - 1 )

#define RING_4_IX0 ( (N_RING_1) + (N_RING_2) + (N_RING_3) )
#define RING_4_IX1 ( (N_RING_1) + (N_RING_2) + (N_RING_3) + (N_RING_4) - 1)

#define RING_5_IX0 ( (N_RING_1) + (N_RING_2) + (N_RING_3) + (N_RING_4) )
#define RING_5_IX1 ( (N_RING_1) + (N_RING_2) + (N_RING_3) + (N_RING_4) + (N_RING_5) - 1 )

#define NUM_LEDS (N_RING_1 + N_RING_2 + N_RING_3 + N_RING_4 + N_RING_5)
#if (NUM_LEDS != FF_LEDFX_NUM_LED)
#  error NUM_LEDS != FF_LEDFX_NUM_LED
#endif

// send data to LEDs
static /*inline*/ void sLedFlush(void)
{
    PIN_HIGH(FLUSH_LED_PIN);
    ws2812Send(STRIP_PIN, ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());
    PIN_LOW(FLUSH_LED_PIN);
}

// update brightness reading
static inline void sLedUpdBright(void)
{
    //PIN_HIGH(FLUSH_LED_PIN);
    //const U1 brightness = (U1)hwAdcGetScaled(HW_ADC_A5, 1, 255);
    //PIN_LOW(FLUSH_LED_PIN);
    //ledfxSetBrightness(brightness);
    ledfxSetBrightness(5);
}


/* ***** LED effects ********************************************************* */

enum { NO_FLUSH = 0, FLUSH_MATRIX = 1 };

// effect state
static union
{
    U1 r[10];

} sFxState;

static U2 sFxTest(const U2 frame)
{
    if (frame == 0)
    {
        sFxState.r[0] = 0; // mode
        sFxState.r[1] = 0; // step
        return NO_FLUSH;
    }
    static const U1 v[] PROGMEM = { 0, 10, 50, 100, 150, 200, 250, 255 };

    if (sFxState.r[1] >= NUMOF(v))
    {
        sFxState.r[0]++;
        sFxState.r[1] = 0;
    }
    if (sFxState.r[0] == 7)
    {
        sFxState.r[0] %= 7;
        return NO_FLUSH;
    }

    const U1 mode = sFxState.r[0];
    const U1 step = sFxState.r[1];

    const U1 vStep = (U1)pgm_read_byte(&v[step]);
    switch (mode)
    {
        case 0:
            DEBUG_F("RGB %3"F_U1, vStep);
            ledfxFillRGB(0, 0, vStep, vStep, vStep);
            break;
        case 1:
            DEBUG_F("R %3"F_U1, vStep);
            ledfxFillRGB(0, 0, vStep, 0, 0);
            break;
        case 2:
            DEBUG_F("G %3"F_U1, vStep);
            ledfxFillRGB(0, 0, 0, vStep, 0);
            break;
        case 3:
            DEBUG_F("B %3"F_U1, vStep);
            ledfxFillRGB(0, 0, 0, 0, vStep);
            break;
        case 4:
            DEBUG_F("RG %3"F_U1, vStep);
            ledfxFillRGB(0, 0, vStep, vStep, 0);
            break;
        case 5:
            DEBUG_F("RB %3"F_U1, vStep);
            ledfxFillRGB(0, 0, vStep, 0, vStep);
            break;
        case 6:
            DEBUG_F("GB %3"F_U1, vStep);
            ledfxFillRGB(0, 0, 0, vStep, vStep);
            break;
    }

    sFxState.r[1]++;
    return FLUSH_MATRIX;
}

static inline U2 sFxNoise1(const U2 frame)
{
    ledfxNoiseRandom(frame == 0 ? true : false, 0, 0, 5);
    return FLUSH_MATRIX;
}

static inline U2 sFxNoise2(const U2 frame)
{
    ledfxNoiseRandomDistinct(frame == 0 ? true : false, 0, 0, 5);
    return FLUSH_MATRIX;
}

static U2 sFxChase(const U2 frame)
{
    UNUSED(frame);
    const U1 sat = 255;
    const U1 val = 255;
    const U1 hue = sFxState.r[0];
    for (U1 ix = RING_1_IX0; ix < (RING_1_IX1 + 1); ix++)
    {
        ledfxSetIxHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_2_IX0; ix < (RING_2_IX1 + 1); ix++)
    {
        ledfxSetIxHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_3_IX0; ix < (RING_3_IX1 + 1); ix++)
    {
        ledfxSetIxHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_4_IX0; ix < (RING_4_IX1 + 1); ix++)
    {
        ledfxSetIxHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_5_IX0; ix < (RING_5_IX1 + 1); ix++)
    {
        ledfxSetIxHSV(ix, hue + (2 * ix), sat, val);
    }
    sFxState.r[0]++;
    return FLUSH_MATRIX;
}

static U2 sFxKaa(const U2 frame)
{
    UNUSED(frame);
    const U1 hue = sFxState.r[0];
    const U1 sat = 255;
    const U1 val = 255;
    ledfxFillHSV(RING_1_IX0, RING_1_IX1, hue,      sat, val);
    ledfxFillHSV(RING_2_IX0, RING_2_IX1, hue + 10, sat, val);
    ledfxFillHSV(RING_3_IX0, RING_3_IX1, hue + 20, sat, val);
    ledfxFillHSV(RING_4_IX0, RING_4_IX1, hue + 30, sat, val);
    ledfxFillHSV(RING_5_IX0, RING_5_IX1, hue + 40, sat, val);
    sFxState.r[0]++;
    return FLUSH_MATRIX;
}

static U2 sFxHueNoise1(const U2 frame)
{
    ledfxNoiseMovingHue(frame == 0 ? true : false, 0, 0, 5, &sFxState.r[0], &sFxState.r[1]);
    return FLUSH_MATRIX;
}

static U2 sFxHueNoise2(const U2 frame)
{
    if (frame == 0)
    {
        sFxState.r[0] = 0;
        sFxState.r[1] = 20; sFxState.r[2] = 127 + 20;
        sFxState.r[3] = 40; sFxState.r[4] = 127 + 30;
        sFxState.r[5] = 60; sFxState.r[6] = 127 + 60;
        sFxState.r[7] = 80; sFxState.r[8] = 127 + 80;
        ledfxClear(0, 0);
    }
    const U1 rnd = hwMathGetRandom();
    ledfxSetIxHSV(0, sFxState.r[0], 255, (rnd & 0x0f) == 0x0f ? 255 : 50);
    ledfxNoiseMovingHue(false, RING_2_IX0, RING_2_IX1, 1, &sFxState.r[1], &sFxState.r[2]);
    ledfxNoiseMovingHue(false, RING_3_IX0, RING_3_IX1, 2, &sFxState.r[3], &sFxState.r[4]);
    ledfxNoiseMovingHue(false, RING_4_IX0, RING_4_IX1, 3, &sFxState.r[5], &sFxState.r[6]);
    ledfxNoiseMovingHue(false, RING_5_IX0, RING_5_IX1, 4, &sFxState.r[7], &sFxState.r[8]);
    sFxState.r[0] += 3;
    return FLUSH_MATRIX;
}

static U2 sFxHueFill(const U2 frame)
{
    UNUSED(frame);
    const U1 hue = sFxState.r[0];
    const U1 sat = 255;
    const U1 val = 255;
    ledfxFillHSV(0, 0, hue, sat, val);
    sFxState.r[0]++;
    return FLUSH_MATRIX;
}

/* ***** capacitive touch "buttons" ****************************************** */

#if (FF_HW_NUM_TICTOC < 1)
#  error Need FF_HW_NUM_TICTOC > 0!
#endif

static OS_SEMAPHORE_t sAppTouchDone;
static volatile U1 svCount;
static volatile U svTime;

ISR(PCINT2_vect)
{
    CLRBITS(PCICR, BIT(PCIE2));
    osIsrEnter();
    svTime = hwToc(0);
    svCount++;
    osSemaphoreGive(&sAppTouchDone);
    osIsrLeave();
}

static void sTouchMeas(void)
{
    const U4 start = osTaskGetTicks();
    //DEBUG("touch start %"F_U4, start);

    osSemaphoreCreate(&sAppTouchDone, 0);
    U4 count = 0;

    CS_ENTER;

    // set send pin low
    PIN_OUTPUT(TOUCH_SEND_PIN);
    PIN_LOW(TOUCH_SEND_PIN);

    // set receive pin
    PIN_INPUT(TOUCH_RECV_PIN);
    PIN_PULLUP_OFF(TOUCH_RECV_PIN);
    PIN_OUTPUT(TOUCH_RECV_PIN);
    hwDelay(10);

    // receive pin to input
    PIN_INPUT(TOUCH_RECV_PIN);

#if 0
    // enable interrupt
    PCMSK2 =           // Pin Change Mask Register 2
        BIT(PCINT19);  // - #TOUCH_RECV_PIN
    SETBITS(PCICR,     // Pin Change Interrupt Control Register
        BIT(PCIE2));   // - Pin Change Interrupt Enable 2 (for PCINT[23:16] pins)
#endif

    // start measuring time
    hwTic(0);
    PIN_HIGH(TOUCH_SEND_PIN);

    while (!PIN_GET(TOUCH_RECV_PIN) && (count < 100000))
    {
        count++;
    }

    PIN_PULLUP_ON(TOUCH_RECV_PIN);
    PIN_OUTPUT(TOUCH_RECV_PIN);
    PIN_HIGH(TOUCH_RECV_PIN);
    PIN_INPUT(TOUCH_RECV_PIN);
    PIN_PULLUP_OFF(TOUCH_RECV_PIN);
    PIN_LOW(TOUCH_SEND_PIN);

    while (PIN_GET(TOUCH_RECV_PIN) && (count < 100000))
    {
        count++;
    }
    svTime = hwToc(0);

    CS_LEAVE;

#if 0
    //PIN_OUTPUT(TOUCH_RECV_PIN);
    PIN_INPUT(TOUCH_RECV_PIN);
    //PIN_PULLUP_ON(TOUCH_RECV_PIN);
    PIN_PULLUP_OFF(TOUCH_RECV_PIN);

    //CLRBITS(DDRD, BIT(PD3));
    //DDRD = 0;
    //CLRBITS(PORTD, BIT(PD3));

    while (ENDLESS)
    {
        osTaskDelay(500);
        DEBUG("pin=%"F_U1, (U1)PIN_GET(TOUCH_RECV_PIN));
        //DEBUG("pin=%"F_U1, (U1)(PIND & BIT(PD3) ? 1 : 0));
        //DEBUG("pin=%"F_U1x, PIND);
    }
#endif
    // wait for response..
    L res = osSemaphoreTake(&sAppTouchDone, 10);

    const U4 stop = osTaskGetTicks();
    DEBUG("touch stop %"F_U4" res=%"F_U1" cnt=%"F_U1" time=%"F_U" count=%"F_U4,
        stop - start, (U1)res, svCount, svTime, count);

    osSemaphoreDelete(&sAppTouchDone);
}


/* ***** application task **************************************************** */


#define FXDURATION 5000
#define FXPERIOD   50

// the effects
static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
  //{ .fxName = "test",      .fxFunc = sFxTest,      .fxPeriod = FXPERIOD, .fxDuration = FXDURATION },
    FXLOOP_INFO("noise1",    sFxNoise1,    FXPERIOD, FXDURATION),
    FXLOOP_INFO("noise2",    sFxNoise2,    FXPERIOD, FXDURATION),
    FXLOOP_INFO("chase",     sFxChase,     FXPERIOD, FXDURATION),
    FXLOOP_INFO("kaa",       sFxKaa,       FXPERIOD, FXDURATION),
    FXLOOP_INFO("huenoise1", sFxHueNoise1, FXPERIOD, FXDURATION),
    FXLOOP_INFO("huenoise2", sFxHueNoise2, FXPERIOD, FXDURATION),
    FXLOOP_INFO("huefill",   sFxHueFill,   FXPERIOD, FXDURATION),
};

// current limiter status
static U2 sAppCurrent;

static void sTimerCb(void *pArg)
{
    UNUSED(pArg);
    PIN_TOGGLE(LED_PIN);
}

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    //CS_ENTER; while (1) {}; CS_LEAVE;

    OS_TIMER_t timer;
    osTimerArm(&timer, sTimerCb, NULL, 2000, 500);

#if 0
    while (ENDLESS)
    {
        osTaskDelay(200);
        sTouchMeas();
        //DEBUG("count=%"F_U1, svCount);
        //hwTic(0);
        //osTaskDelay(3);
        //DEBUG("delay=%"F_U, hwToc(0));

    }
#endif

    // run the test program once
    sFxTest(0);
    while (sFxTest(1))
    {
        sLedUpdBright();
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(50);
    }
    ledfxClear(0, 0);
    sLedFlush();

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    // keep running...
    while (ENDLESS)
    {
        //PIN_TOGGLE(LED_PIN);

        const U2 res = fxloopRun(false);

        // update matrix?
        if (res == FLUSH_MATRIX)
        {
            sLedUpdBright();
            ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
            sLedFlush();
        }

        // delay until it's time to run the next iteration of the effect
        fxloopWait();

    } // ENDLESS
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    const int n = snprintf_P(str, size, PSTR("sAppCurrent=%"F_U2" "),
        sAppCurrent);
    fxloopStatus(&str[n], size - n);
}


//@}
// eof

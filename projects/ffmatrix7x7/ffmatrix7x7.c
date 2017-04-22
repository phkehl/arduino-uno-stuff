/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup PROJECTS_FFMATRIX7X7
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

#include "ffmatrix7x7.h"

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
    DEBUG("ex3: init");

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

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

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

// effect state
static union
{
    U1 r[10];

} sFxState;

static inline L sFxNoOp(const L init)
{
    if (init)
    {
        ledfxClear(0, 0);
    }
    return init;
}

static L sFxTest(const L init)
{
    if (init)
    {
        sFxState.r[0] = 0; // mode
        sFxState.r[1] = 0; // step
        return false;
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
        return false;
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
    return true;
}

static inline L sFxNoise1(const L init)
{
    ledfxNoiseRandom(init, 0, 0, 5);
    return true;
}

static inline L sFxNoise2(const L init)
{
    ledfxNoiseRandomDistinct(init, 0, 0, 5);
    return true;
}

static L sFxChase(const L init)
{
    UNUSED(init);
    const U1 sat = 255;
    const U1 val = 255;
    const U1 hue = sFxState.r[0];
    for (U1 ix = RING_1_IX0; ix < (RING_1_IX1 + 1); ix++)
    {
        ledfxSetHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_2_IX0; ix < (RING_2_IX1 + 1); ix++)
    {
        ledfxSetHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_3_IX0; ix < (RING_3_IX1 + 1); ix++)
    {
        ledfxSetHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_4_IX0; ix < (RING_4_IX1 + 1); ix++)
    {
        ledfxSetHSV(ix, hue + (2 * ix), sat, val);
    }
    for (U1 ix = RING_5_IX0; ix < (RING_5_IX1 + 1); ix++)
    {
        ledfxSetHSV(ix, hue + (2 * ix), sat, val);
    }
    sFxState.r[0]++;
    return true;
}

static L sFxKaa(const L init)
{
    UNUSED(init);
    const U1 hue = sFxState.r[0];
    const U1 sat = 255;
    const U1 val = 255;
    ledfxFillHSV(RING_1_IX0, RING_1_IX1, hue,      sat, val);
    ledfxFillHSV(RING_2_IX0, RING_2_IX1, hue + 10, sat, val);
    ledfxFillHSV(RING_3_IX0, RING_3_IX1, hue + 20, sat, val);
    ledfxFillHSV(RING_4_IX0, RING_4_IX1, hue + 30, sat, val);
    ledfxFillHSV(RING_5_IX0, RING_5_IX1, hue + 40, sat, val);
    sFxState.r[0]++;
    return true;
}

static L sFxHueNoise1(const L init)
{
    ledfxNoiseMovingHue(init, 0, 0, 5, &sFxState.r[0], &sFxState.r[1]);
    return true;
}

static L sFxHueNoise2(const L init)
{
    if (init)
    {
        sFxState.r[0] = 0;
        sFxState.r[1] = 20; sFxState.r[2] = 127 + 20;
        sFxState.r[3] = 40; sFxState.r[4] = 127 + 30;
        sFxState.r[5] = 60; sFxState.r[6] = 127 + 60;
        sFxState.r[7] = 80; sFxState.r[8] = 127 + 80;
        ledfxClear(0, 0);
    }
    const U1 rnd = hwMathGetRandom();
    ledfxSetHSV(0, sFxState.r[0], 255, (rnd & 0x0f) == 0x0f ? 255 : 50);
    ledfxNoiseMovingHue(false, RING_2_IX0, RING_2_IX1, 1, &sFxState.r[1], &sFxState.r[2]);
    ledfxNoiseMovingHue(false, RING_3_IX0, RING_3_IX1, 2, &sFxState.r[3], &sFxState.r[4]);
    ledfxNoiseMovingHue(false, RING_4_IX0, RING_4_IX1, 3, &sFxState.r[5], &sFxState.r[6]);
    ledfxNoiseMovingHue(false, RING_5_IX0, RING_5_IX1, 4, &sFxState.r[7], &sFxState.r[8]);
    sFxState.r[0] += 3;
    return true;
}

static L sFxHueFill(const L init)
{
    UNUSED(init);
    const U1 hue = sFxState.r[0];
    const U1 sat = 255;
    const U1 val = 255;
    ledfxFillHSV(0, 0, hue, sat, val);
    sFxState.r[0]++;
    return true;
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

// effect function t ype
typedef L (*FX_FUNC_t)(const L);

// effect info
typedef struct FX_INFO_s
{
    CH        name[16];  // loop name
    FX_FUNC_t func;      // loop function
    U4        minPeriod; // minimum useful update period (1 / refresh rate)
    U4        maxPeriod; // maximum useful update period (1 / refresh rate)
    U4        autoTime;  // how long to play in auto mode (0 = don't use in auto-mode)
} FX_INFO_t;

// the loop of effects
static const FX_INFO_t skFxInfo[] PROGMEM =
{
#define _T 30000
    { "noop",        sFxNoOp,            10,  1000,   0 },
    { "test",        sFxTest,            100, 5000,   0 },
    { "noise1",      sFxNoise1,          10,  1000,  _T },
    { "noise2",      sFxNoise2,          10,  1000,  _T },
    { "chase",       sFxChase,           10,  1000,  _T },
    { "kaa",         sFxKaa,             10,  1000,  _T },
    { "huenoise1",   sFxHueNoise1,       10,  1000,  _T },
    { "huenoise2",   sFxHueNoise2,       10,  1000,  _T },
    { "huefill",     sFxHueFill,         10,  1000,  _T },


//    { "",      sFx,          10,  1000,  30000 },
//    { "",      sFx,          10,  1000,  30000 },
//    { "",      sFx,          10,  1000,  30000 },

};

#define FX_INFO_COUNT         NUMOF(skFxInfo)
#define FX_INFO_NAME(ix)      (/* STFU */const wchar_t *)&skFxInfo[ix].name
#define FX_INFO_NAMEFMT       "%S"
#define FX_INFO_FUNC(ix)      (FX_FUNC_t)pgm_read_word(&skFxInfo[ix].func)
#define FX_INFO_MINPERIOD(ix) (U4)pgm_read_dword(&skFxInfo[ix].minPeriod)
#define FX_INFO_MAXPERIOD(ix) (U4)pgm_read_dword(&skFxInfo[ix].maxPeriod)
#define FX_INFO_AUTOTIME(ix)  (U4)pgm_read_dword(&skFxInfo[ix].autoTime)

// loop state
typedef enum LOOP_RUN_e
{
    LOOP_RUN_INIT,        // initialise effect
    LOOP_RUN_PLAY,        // play effect
    LOOP_RUN_NEXT,        // select next effect to play
} LOOP_RUN_t;

// loop status
typedef struct LOOP_STATUS_s
{
    LOOP_RUN_t loopState;   // current loop state
    L          autoPlay;    // are we in auto-play mode?
    U          loopIx;      // currently selected effect
    FX_FUNC_t  progFunc;    // currently selected effect function
    U4         autoTime;    // timeout for auto-play
    U4         autoDelay;   // delay for auto-play
    U4         frame;       // frame counter
    U4         frameDrop;   // frame drop counter
    U4         period;      // refresh period
} LOOP_STATUS_t;

// app status
static LOOP_STATUS_t sLoopStatus;

// current limiter status
static U2 sAppCurrent;


#include "atomthreads.h"
static void sTimerCb(void *pArg)
{
#if 1
    UNUSED(pArg);
    PIN_TOGGLE(LED_PIN);

#else
    PIN_TOGGLE(LED_PIN);
    OS_TIMER_t *pTimer = (OS_TIMER_t *)pArg;
    pTimer->cb_ticks = 500;
    atomTimerRegister(pTimer);
#endif
}


// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    //CS_ENTER; while (1) {}; CS_LEAVE;

    OS_TIMER_t timer;
    osTimerArm(&timer, sTimerCb, NULL, 2000, 500);
    //osTimerArm(&timer, sTimerCb, &timer, 2000, 500);

#if 0
    while (ENDLESS)
    {
        osTaskDelay(1000);
        DEBUG("repeat=%"F_U2, timer.cb_repeat);
    }
#endif

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

#if 0
    // run the test program once
    sFxTest(true);
    while (sFxTest(false))
    {
        sLedUpdBright();
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(50);
    }
    ledfxClear(0, 0);
    sLedFlush();
#endif


    // print the effects that available
    PRINT_F("loop: the menu today:");
    for (U ix = 0; ix < FX_INFO_COUNT; ix++)
    {
        PRINT_F("loop: %2"F_U"%S "FX_INFO_NAMEFMT,
            ix, FX_INFO_AUTOTIME(ix) ? PSTR("*") : PSTR(" "), FX_INFO_NAME(ix));
        // FIXME: why doesn't PRINT('%c', .. ? '*' : ' ') not work?
    }

#if 0
    // temporary
    sFxHueNoise2(true);
    while (sFxHueNoise2(false))
    {
        sLedUpdBright();
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(50);
    }
#endif

    // setup effect loop status
    memset(&sLoopStatus, 0, sizeof(sLoopStatus));
    sLoopStatus.loopState = LOOP_RUN_NEXT;
    sLoopStatus.autoPlay  = true;
    sLoopStatus.loopIx    = -1;
    static U4 msss;
    msss = osTaskGetTicks();
    static L doUpdate;
    doUpdate = false;

#if 0
    {
        U n = 10;
        while (n--)
        {
            PIN_TOGGLE(LED_PIN);
            osTaskDelay(500);
        }
    }
#endif

    // keep running...
    while (ENDLESS)
    {
        //PIN_TOGGLE(LED_PIN);

        switch (sLoopStatus.loopState)
        {
            /* ***** select next program (when in auto-mode) ***************** */

            case LOOP_RUN_NEXT:
                while (ENDLESS)
                {
                    // try next
                    sLoopStatus.loopIx++;

                    // wrap
                    sLoopStatus.loopIx %= FX_INFO_COUNT;

                    // skip ix 0 in autoMode
                    //if (sLoopStatus.autoPlay && (sLoopStatus.loopIx == 0))
                    //{
                    //    continue;
                    //}

                    // got a suitable effect?
                    sLoopStatus.autoTime = FX_INFO_AUTOTIME(sLoopStatus.loopIx);
                    if (sLoopStatus.autoTime > 0)
                    {
//                        loopParamSet(LOOP_PARAM_PROG, sLoopStatus.loopIx, 0,  NUMOF(skProgInfo) - 1);
                        break;
                    }
                }

                // next, go clear..
                sLoopStatus.loopState = LOOP_RUN_INIT;
                break;


            /* ***** initialise effect *************************************** */

            case LOOP_RUN_INIT:

                sLoopStatus.progFunc = FX_INFO_FUNC(sLoopStatus.loopIx);
                sLoopStatus.autoDelay = 0;
                sLoopStatus.autoPlay  = true; // FIXME loopParamGet(LOOP_PARAM_MODE) ? TRUE : FALSE;
                sLoopStatus.frame     = 0;
                sLoopStatus.frameDrop = 0;
                sLoopStatus.autoTime  = FX_INFO_AUTOTIME(sLoopStatus.loopIx);

                NOTICE("loop: " FX_INFO_NAMEFMT " (%" F_I ") " FX_INFO_NAMEFMT " %" F_U4,
                      FX_INFO_NAME(sLoopStatus.loopIx), sLoopStatus.loopIx,
                      sLoopStatus.autoPlay ? PSTR("auto") : PSTR("user"), sLoopStatus.autoTime);
                // FIXME: update auto mode LED here

                // run program initialisation
                sLoopStatus.frame++;
                PIN_HIGH(FLUSH_LED_PIN);
                doUpdate = sLoopStatus.progFunc(true);
                PIN_LOW(FLUSH_LED_PIN);

                // go play..
                sLoopStatus.loopState = LOOP_RUN_PLAY;
                break;


            /* ***** play effect ********************************************* */

            case LOOP_RUN_PLAY:

                // run effect (render next frame)
                sLoopStatus.frame++;
                doUpdate = sLoopStatus.progFunc(false);

                // switch to next loop by user choice?
//                const U1 paramProg = loopParamGet(LOOP_PARAM_PROG);
//                if (sLoopStatus.loopIx != paramProg)
//                {
//                    sLoopStatus.loopIx = paramProg;
//                    sLoopStatus.loopState = LOOP_RUN_CLEAR;
//                    // disable auto-mode
//                    loopParamAdj(LOOP_PARAM_MODE, -1);
//                    break;
//                }
//                // switch to next loop because we're in auto-mode and the time's up
//                else
                    if (sLoopStatus.autoPlay && (sLoopStatus.autoDelay >= sLoopStatus.autoTime))
                {
                    sLoopStatus.loopState = LOOP_RUN_NEXT;
                }

                break;
        }

        // update matrix?
        if (doUpdate)
        {
            sLedUpdBright();
            ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
            sLedFlush();
        }

        // delay to achieve desired update period (refresh rate)
        if (sLoopStatus.loopState != LOOP_RUN_NEXT)
        {
            // update auto-mode setting?
//            const L autoMode = loopParamGet(LOOP_PARAM_MODE) ? TRUE : FALSE;
//            if (sLoopStatus.autoPlay != autoMode)
//            {
//                sLoopStatus.autoPlay = autoMode;
//                hwSetModeDisplay(sLoopStatus.loopIx, sLoopStatus.autoPlay);
//            }

            // target refresh rate (period in [ms])
            const U4 period = 50; //loopParamGetScale(LOOP_PARAM_SPEED, // FIXME: slow?
                                  //              FX_INFO_MINPERIOD(sLoopStatus.loopIx),
                                  //              FX_INFO_MAXPERIOD(sLoopStatus.loopIx));
            sLoopStatus.period = period;

            // delay until next frame
            osTaskDelayUntil(&msss, period);

            // update auto-play time
            sLoopStatus.autoDelay += period;

            // if we were too late, drop one frame and sleep until next period
            const U4 msssNow = osTaskGetTicks();
            if ( (msssNow - msss) > 10 ) // other tasks may schedule in before us so that we may be slightly late
            {
                const U4 dt = (msssNow - msss);
                const U4 numDropped = (dt / period) + 1;
                WARNING("loop: " FX_INFO_NAMEFMT " %" F_U4 " frames dropped! %" F_U4 "+%" F_U4 "ms",
                        FX_INFO_NAME(sLoopStatus.loopIx),
                        numDropped, period, dt);
                sLoopStatus.frameDrop += numDropped;
                sLoopStatus.frame += numDropped;
                msss = (msssNow / period) * period;
                osTaskDelayUntil(&msss, period);
                sLoopStatus.autoDelay += period;
            }
        }
    } // ENDLESS
}


#if 0
static U1 sAppMode;
static U2 sAppCurrent;
static U4 sAppTick;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // keep running...
    while (ENDLESS)
    {
        osTaskDelay(50);
    }

    sLedUpdBright();
    sAppTest(50, 0, 0);

    sAppTick = osTaskGetTicks();
    L init = true;
    const U4 interval = 50;
    const U4 period = 30000;
    static U4 nIntervalsToGo = period / interval;
    sAppMode = 0;

    static U1 r0, r1, r2, r3, r4, r5, r6, r7, r8;

    // keep running...
    while (ENDLESS)
    {
        // run at 50 Hz
        osTaskDelayUntil(&sAppTick, interval);

        // set the LED
        PIN_HIGH(LED_PIN);

        // change mode every now and then
        nIntervalsToGo--;
        if (nIntervalsToGo == 0)
        {
            nIntervalsToGo = period / interval;
            sAppMode++;
            sAppMode %= 7;
            init = true;
        }
        if (init)
        {
            NOTICE("app: mode=%"F_U1, sAppMode);
            ledfxClear(0, 0);
        }

        else if (sAppMode == 4)
        {
        }
        else if (sAppMode == 5)
        {
        }
        else if (sAppMode == 6)
        {
        }

        // update brightness reading and send data to the LEDs
        sLedUpdBright();
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        init = false;

        // set the LED
        PIN_LOW(LED_PIN);

    } // ENDLESS
}
#endif

#if 0
static void sAppTest(const U4 delay, const U1 ix0, const U1 ix1)
{
    const U1 v[] = { 0, 10, 50, 100, 150, 200, 250, 255 };
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("RGB %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, v[i], v[i], v[i]);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("R %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, v[i], 0, 0);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("G %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, 0, v[i], 0);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("B %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, 0, 0, v[i]);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("RG %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, v[i], v[i], 0);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("RB %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, v[i], 0, v[i]);
        ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
        sLedFlush();
        osTaskDelay(delay);
    }
    for (U1 i = 0; i < NUMOF(v); i++)
    {
        DEBUG("GB %3"F_U1, v[i]);
        ledfxFillRGB(ix0, ix1, 0, v[i], v[i]);
        sLedFlush();
        osTaskDelay(delay);
    }
    ledfxClear(0, 0);
    sLedFlush();
}
#endif

/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf(str, size, "sAppCurrent=%"F_U2,
        sAppCurrent);

    //const R4 hz = sLoopStatus.period ? 1000.0f / (R4)sLoopStatus.period : 0.0f;
    const U1 hz = sLoopStatus.period ? 1000 / sLoopStatus.period : 0;
    snprintf(str, size, "%02"F_U" "FX_INFO_NAMEFMT" %c:%"F_U4"/%"F_U4" %"F_U4"/%"F_U4" %"F_U4"ms/%"F_U1"Hz",
        sLoopStatus.loopIx,
        FX_INFO_NAME(sLoopStatus.loopIx),
        sLoopStatus.autoPlay ? 'A' : 'M',
        sLoopStatus.autoDelay, sLoopStatus.autoTime,
        sLoopStatus.frameDrop, sLoopStatus.frame,
        sLoopStatus.period,
        hz
        );
    str[size-1] = '\0';
}


//@}
// eof

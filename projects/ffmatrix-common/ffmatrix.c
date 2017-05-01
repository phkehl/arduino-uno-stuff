/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix common stuff (see \ref PROJECTS_FFMATRIX_COMMON)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX_COMMON
    @{
*/

#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "unopins.h"       // ff: Arduino Uno pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects
#include "fxloop.h"        // ff: effects loops

#include "ffmatrix.h"

#if (!defined FFMATRIX_MODEL) || (!defined FFMATRIX_FLUSH_LED) || (!defined FFMATRIX_SPEED_POT) || (!defined FFMATRIX_BRIGHT_POT)
#  error missing configuration
#endif


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);
static void sLedFlush(void);

// initialise the user application
void appInit(void)
{
    DEBUG("ffmatrix7x7: init");

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);

    // matrix access debugging
    PIN_OUTPUT(FFMATRIX_FLUSH_LED);
    PIN_LOW(FFMATRIX_FLUSH_LED);

    // initialise LED matrix
    ws2801Init();
    ledfxClear(0, 0);
    sLedFlush();

    // enable interrupt (to register button presses)
    PIN_INPUT(_PD2);
    PIN_PULLUP_ON(_PD2);
    SETBITS(EICRA, BIT(ISC01));   // falling-edge triggers interrupt
    //CLRBITS(EICRA, BIT(ISC01) | BIT(ISC00)); // low-level triggers interrupt
    SETBITS(EIMSK, BIT(INT0));  // enable INT0 interrupt

    // mode display (binary display of a couple of LEDs to indicate the effect number playing)
#ifdef FFMATRIX_MODE_PIN_0
    PIN_OUTPUT(FFMATRIX_MODE_PIN_0);
    PIN_LOW(FFMATRIX_MODE_PIN_0);
#endif
#ifdef FFMATRIX_MODE_PIN_1
    PIN_OUTPUT(FFMATRIX_MODE_PIN_1);
    PIN_LOW(FFMATRIX_MODE_PIN_1);
#endif
#ifdef FFMATRIX_MODE_PIN_2
    PIN_OUTPUT(FFMATRIX_MODE_PIN_2);
    PIN_LOW(FFMATRIX_MODE_PIN_2);
#endif
#ifdef FFMATRIX_MODE_PIN_3
    PIN_OUTPUT(FFMATRIX_MODE_PIN_3);
    PIN_LOW(FFMATRIX_MODE_PIN_3);
#endif

    // initialise ADC
    hwAdcInit(FFMATRIX_SPEED_POT | FFMATRIX_BRIGHT_POT, false);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[350];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application functions *********************************************** */

// current limiter status
static uint16_t sCurrent;

// send data to LEDs
static /*inline*/ void sLedFlush(void)
{
    PIN_HIGH(FFMATRIX_FLUSH_LED);

    ledfxLimitCurrent(FFMATRIX_MA_PER_LED, FFMATRIX_PSU_MAX_MA, &sCurrent);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

    PIN_LOW(FFMATRIX_FLUSH_LED);
}


/* ***** LED effects ********************************************************* */

enum { NO_FLUSH = 0, FLUSH_MATRIX = 1 };

// effect state "registers"
static union
{
    uint8_t u[12];
    float f[3];
    LEDFX_RAIN_t rain;
    LEDFX_STAR_t stars[FF_LEDFX_NUM_LED / 5];
} sFxState;

//static inline uint16_t sFxNoise1(const uint16_t frame)
//{
//    ledfxNoiseRandom(frame == 0 ? true : false, 0, 0, 5);
//    return FLUSH_MATRIX;
//}

static inline uint16_t sFxNoise2(const uint16_t frame)
{
    ledfxNoiseRandomDistinct(frame == 0 ? true : false, 0, 0, 5);
    return FLUSH_MATRIX;
}

static uint16_t sFxHueNoise(const uint16_t frame)
{
    ledfxNoiseMovingHue(frame == 0 ? true : false, 0, 0, 5, &sFxState.u[0], &sFxState.u[1]);
    return FLUSH_MATRIX;
}

static uint16_t sFxKaasEye(const uint16_t frame)
{
    ledfxConcentricHueFlow(frame == 0 ? true : false, 1, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static uint16_t sFxPlasma(const uint16_t frame)
{
    ledfxPlasma(frame == 0 ? true : false, &sFxState.f[0]);
    return FLUSH_MATRIX;
}

static uint16_t sFxRotor(const uint16_t frame)
{
    ledfxRotor(frame == 0 ? true : false, &sFxState.f[0], &sFxState.f[1]);
    return FLUSH_MATRIX;
}

static uint16_t sFxRainbow(const uint16_t frame)
{
    ledfxRainbow(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static uint16_t sFxRain(const uint16_t frame)
{
    ledfxRain(frame == 0 ? true : false, &sFxState.rain);
    return FLUSH_MATRIX;
}

//static uint16_t sFxHueSweep(const uint16_t frame)
//{
//    ledfxHueSweep(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
//    return FLUSH_MATRIX;
//}

static uint16_t sFxStars(const uint16_t frame)
{
    ledfxStars(frame == 0 ? true : false, sFxState.stars, NUMOF(sFxState.stars));
    return FLUSH_MATRIX;
}

//static uint16_t sFxStrobo(const uint16_t frame)
//{
//    ledfxStrobo(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
//    return FLUSH_MATRIX;
//}

static uint16_t sFxWaves(const uint16_t frame)
{
    ledfxWaves(frame == 0 ? true : false, &sFxState.u[0], &sFxState.f[1], &sFxState.f[2]);
    return FLUSH_MATRIX;
}

static uint16_t sFxDiagonal(const uint16_t frame)
{
    ledfxDiagonal(frame == 0 ? true : false, &sFxState.u[0]);
    return FLUSH_MATRIX;
}


/* ***** application task **************************************************** */

#define FXDURATION (uint32_t)120000

// the effects
static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
  //FXLOOP_INFO("noise 1",    sFxNoise1,    10, 100, FXDURATION),
    FXLOOP_INFO("diagonal",   sFxDiagonal,  10, 250, FXDURATION),
    FXLOOP_INFO("noise 2",    sFxNoise2,    10, 100, FXDURATION),
    FXLOOP_INFO("rotor",      sFxRotor,     10, 250, FXDURATION), // frame 7-10ms
    FXLOOP_INFO("plasma",     sFxPlasma,    50, 250, FXDURATION), // frame ~50ms
    FXLOOP_INFO("kaa's eye",  sFxKaasEye,   15, 150, FXDURATION),
    FXLOOP_INFO("rain",       sFxRain,      30, 250, FXDURATION),
    FXLOOP_INFO("rainbow",    sFxRainbow,   15, 250, FXDURATION),
    FXLOOP_INFO("stars",      sFxStars,      5, 250, FXDURATION),
    FXLOOP_INFO("hue noise",  sFxHueNoise,  30, 250, FXDURATION),
    FXLOOP_INFO("waves",      sFxWaves,     50, 250, FXDURATION),
  //FXLOOP_INFO("strobo",     sFxStrobo,    FXPERIOD, FXDURATION),
  //FXLOOP_INFO("huesweep",   sFxHueSweep,  FXPERIOD, FXDURATION),
};

#define TRANSTIME 750

static uint8_t sBrightness = 50;
static uint8_t sSpeed = 50;
static volatile bool svButtonPressed;

ISR(INT0_vect) // external interrupt 0
{
    static volatile uint32_t msss0;
    osIsrEnter();
    const uint32_t msss1 = osTaskGetTicks();
    if ( (msss1 - msss0) > 150 )
    {
        svButtonPressed = true;
    }
    msss0 = msss1;
    osIsrLeave();
}

static void sSetModeDisplay(const uint8_t mode)
{
#ifdef FFMATRIX_MODE_PIN_3
    PIN_SET(FFMATRIX_MODE_PIN_3, mode & BIT(3));
#endif
#ifdef FFMATRIX_MODE_PIN_2
    PIN_SET(FFMATRIX_MODE_PIN_2, mode & BIT(2));
#endif
#ifdef FFMATRIX_MODE_PIN_1
    PIN_SET(FFMATRIX_MODE_PIN_1, mode & BIT(1));
#endif
#ifdef FFMATRIX_MODE_PIN_0
    PIN_SET(FFMATRIX_MODE_PIN_0, mode & BIT(0));
#else
    UNUSED(mode);
#endif
}

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    int8_t doTransition = FF_LEDFX_NUM_Y;

    while (ENDLESS)
    {
        // button pressed? again?
        while (svButtonPressed)
        {
            // display effect number
            const uint8_t fxNum = fxloopCurrentlyPlaying();
            const uint8_t brightness = sBrightness < 100 ? 50 + sBrightness : sBrightness;
            ledfxSetBrightness(brightness);
            const uint8_t hue = (fxNum % 10) * (255/10);
            ledfxFillHSV(0, 0, hue + 128, 255, 50);
            ledfxDigit(fxNum, (FF_LEDFX_NUM_X - 4 + 1) / 2, (FF_LEDFX_NUM_X - 7 + 1) / 2, hue, 255, 255);
            if (fxNum >= 10)
            {
                ledfxSetMatrixHSV(0, 0, hue, 255, 255);
            }
            sLedFlush();

            // display for a while and repeat if the button has been pressed again
            svButtonPressed = false;
            uint8_t n = 15;
            while (n--)
            {
                if (svButtonPressed)
                {
                    // move effect loop to next effect (but don't play it yet)
                    fxloopRun(true);
                    doTransition = -1;
                    break;
                }
                osTaskDelay(50);
            }
        }

        // do transition on effect end
        if (doTransition < 0)
        {
            uint16_t y = FF_LEDFX_NUM_Y;
            while (y--)
            {
                uint16_t x = FF_LEDFX_NUM_X;
                while (x--)
                {
                    ledfxSetMatrixRGB(x, y, 0, 0, 0);
                }
                sLedFlush();
                osTaskDelay(TRANSTIME / FF_LEDFX_NUM_Y);
            }
            doTransition = FF_LEDFX_NUM_Y;
        }


        // render next frame
        const uint16_t res = fxloopRun(false);

        // update matrix?
        if (res == FLUSH_MATRIX)
        {
            // do transition on effect start
            // FIXME: make this last TRANSTIME [ms]
            if (doTransition > 0)
            {
                sSetModeDisplay( fxloopCurrentlyPlaying() );
                uint16_t y = doTransition--;
                while (y--)
                {
                    uint16_t x = FF_LEDFX_NUM_X;
                    while (x--)
                    {
                        ledfxSetMatrixRGB(x, FF_LEDFX_NUM_Y - y, 0, 0, 0);
                    }
                }
            }

            ledfxSetBrightness(sBrightness);
            sLedFlush();
        }

        // read grey pot for brightness, convert from linear reading to exponential brightness level
        // (i.e. start increasing brightness slowly; use smaller brightness steps at the lower end)
        const float pot1 = hwAdcGetScaled(FFMATRIX_BRIGHT_POT, 1, 55374); // log(254) * 10000
        sBrightness = (uint8_t)expf(pot1 / 10000); // 1.0..254.01 --> 1..254

        // read black pot, adjust effect speed
        const int32_t pot2 = hwAdcGetScaled(FFMATRIX_SPEED_POT, 0, 100); // black
        sSpeed = (uint8_t)pot2;

        // delay until it's time to run the next iteration of the effect
        if ( fxloopWait(sSpeed) )
        {
            doTransition = -1;
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    const int n = snprintf_P(str, size, PSTR("%"PRIu16"mA bri=%"PRIu8" spd=%"PRIu8" "),
        sCurrent, sBrightness, sSpeed);
    fxloopStatus(&str[n], size - n);
    str[size-1] = '\0';
}


//@}
// eof

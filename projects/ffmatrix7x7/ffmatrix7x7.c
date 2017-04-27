/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX7X7
    @{
*/

#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions

//#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

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

#include "ffmatrix7x7.h"


/* ***** application init **************************************************** */

// LED lit while data is written to the LED strip
#define FLUSH_LED_PIN _PC5

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
    PIN_OUTPUT(FLUSH_LED_PIN);
    PIN_LOW(FLUSH_LED_PIN);

    // initialise LED matrix
    ws2801Init();
    ledfxClear(0, 0);
    sLedFlush();

    // initialise ADC
    hwAdcInit(HW_ADC_PC3 | HW_ADC_PC4, false);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application functions *********************************************** */

// [mA] per LED if fully lit
#define MA_PER_LED 60

// maximum [mA] the power supply can deliver
#define PSU_MAX_MA 2500

// current limiter status
static uint16_t sCurrent;

// send data to LEDs
static /*inline*/ void sLedFlush(void)
{
    PIN_HIGH(FLUSH_LED_PIN);

    ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sCurrent);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

    PIN_LOW(FLUSH_LED_PIN);
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


#define FXDURATION (uint32_t)60000

// TODO: fix duration

// the effects
static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
  //FXLOOP_INFO("noise 1",    sFxNoise1,    10, 100, 10000),
    FXLOOP_INFO("diagonal",   sFxDiagonal,  10, 250, FXDURATION), // 10..250
    FXLOOP_INFO("noise 2",    sFxNoise2,    10, 100, FXDURATION),
    FXLOOP_INFO("rotor",      sFxRotor,     10, 250, FXDURATION), // 10..250, frame 7-10ms
    FXLOOP_INFO("plasma",     sFxPlasma,    50, 250, FXDURATION), // 50..250, frame ~50ms
    FXLOOP_INFO("kaa's eye",  sFxKaasEye,   15, 150, FXDURATION), // 15..150
    FXLOOP_INFO("rain",       sFxRain,      30, 250, FXDURATION), // 30..250
    FXLOOP_INFO("rainbow",    sFxRainbow,   15, 250, FXDURATION), // 15..250
    FXLOOP_INFO("stars",      sFxStars,      5, 250, FXDURATION), // 5..250
    FXLOOP_INFO("hue noise",  sFxHueNoise,  30, 250, FXDURATION), // 30..250
    FXLOOP_INFO("waves",      sFxWaves,     50, 250, FXDURATION), // 50..250
  //FXLOOP_INFO("strobo",     sFxStrobo,    FXPERIOD, 60000),
  //FXLOOP_INFO("huesweep",   sFxHueSweep,  FXPERIOD, FXDURATION),
};

static uint8_t sBrightness = 50;
static uint8_t sSpeed = 50;

// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);


    while (ENDLESS)
    {
        const uint16_t res = fxloopRun(false);

        ledfxSetBrightness(sBrightness);

        // update matrix?
        if (res == FLUSH_MATRIX)
        {
            sLedFlush();
        }

        // read grey pot for brightness, convert to linear reading to exponential brightness
        const float pot1 = hwAdcGetScaled(HW_ADC_PC4, 1, 55374); // log(254) * 10000
        sBrightness = (uint8_t)expf(pot1 / 10000); // 1.0..254.01 --> 1..254

        // read black pot, adjust effect speed
        const int32_t pot2 = hwAdcGetScaled(HW_ADC_PC3, 100, 0); // black
        sSpeed = (uint8_t)pot2;

        // delay until it's time to run the next iteration of the effect
        /*const bool willChange = */fxloopWait(sSpeed);
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

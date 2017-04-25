/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2014-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup PROJECTS_FFMATRIX7X7
    @{
*/

#include <string.h>        // libc: string operations

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
}

// starts the user application task
void appCreateTask(void)
{
    static U1 stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}


/* ***** application functions *********************************************** */

// [mA] per LED if fully lit
#define MA_PER_LED 60

// maximum [mA] the power supply can deliver
#define PSU_MAX_MA 2500

// current limiter status
static U2 sAppCurrent;

// send data to LEDs
static /*inline*/ void sLedFlush(void)
{
    PIN_HIGH(FLUSH_LED_PIN);

    ledfxLimitCurrent(MA_PER_LED, PSU_MAX_MA, &sAppCurrent);
    ws2801Send(ledfxGetFrameBuffer(), ledfxGetFrameBufferSize());

    PIN_LOW(FLUSH_LED_PIN);
}


/* ***** LED effects ********************************************************* */

enum { NO_FLUSH = 0, FLUSH_MATRIX = 1 };

// effect state "registers"
static union
{
    U1 u[12];
    R4 f[3];
    LEDFX_RAIN_t rain;
    LEDFX_STAR_t stars[FF_LEDFX_NUM_LED / 5];
} sFxState;

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

static U2 sFxHueNoise1(const U2 frame)
{
    ledfxNoiseMovingHue(frame == 0 ? true : false, 0, 0, 5, &sFxState.u[0], &sFxState.u[1]);
    return FLUSH_MATRIX;
}

static U2 sFxHueFill(const U2 frame)
{
    UNUSED(frame);
    const U1 hue = sFxState.u[0];
    const U1 sat = 255;
    const U1 val = 255;
    ledfxFillHSV(0, 0, hue, sat, val);
    sFxState.u[0]++;
    return FLUSH_MATRIX;
}

static U2 sFxKaasEye(const U2 frame)
{
    ledfxConcentricHueFlow(frame == 0 ? true : false, 1, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static U2 sFxPlasma(const U2 frame)
{
    ledfxPlasma(frame == 0 ? true : false, &sFxState.f[0]);
    return FLUSH_MATRIX;
}

static U2 sFxRotor(const U2 frame)
{
    ledfxRotor(frame == 0 ? true : false, &sFxState.f[0], &sFxState.f[1]);
    return FLUSH_MATRIX;
}

static U2 sFxRainbow(const U2 frame)
{
    ledfxRainbow(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static U2 sFxRain(const U2 frame)
{
    ledfxRain(frame == 0 ? true : false, &sFxState.rain);
    return FLUSH_MATRIX;
}

static U2 sFxHueSweep(const U2 frame)
{
    ledfxHueSweep(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static U2 sFxStars(const U2 frame)
{
    ledfxStars(frame == 0 ? true : false, sFxState.stars, NUMOF(sFxState.stars));
    return FLUSH_MATRIX;
}

static U2 sFxStrobo(const U2 frame)
{
    ledfxStrobo(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
    return FLUSH_MATRIX;
}

static U2 sFxWaves(const U2 frame)
{
    ledfxWaves(frame == 0 ? true : false, &sFxState.u[0], &sFxState.f[1], &sFxState.f[2]);
    return FLUSH_MATRIX;
}

/* ***** application task **************************************************** */


#define FXDURATION (U4)10000
#define FXPERIOD   50

// the effects
static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
  //{ .fxName = "test",      sFxTest,      FXPERIOD, FXDURATION),
  //FXLOOP_INFO("strobo",    sFxStrobo,    FXPERIOD, 60000),
    FXLOOP_INFO("waves",     sFxWaves,     FXPERIOD, FXDURATION),
    FXLOOP_INFO("stars",     sFxStars,     FXPERIOD, FXDURATION),
    FXLOOP_INFO("huesweep",  sFxHueSweep,  FXPERIOD, FXDURATION),
    FXLOOP_INFO("rain",      sFxRain,      FXPERIOD, FXDURATION),
    FXLOOP_INFO("rainbow",   sFxRainbow,   FXPERIOD, FXDURATION),
    FXLOOP_INFO("plasma",    sFxPlasma,    FXPERIOD, FXDURATION), // frame ~50ms
    FXLOOP_INFO("rotor",     sFxRotor,     FXPERIOD, FXDURATION), // frame 7-10ms
    FXLOOP_INFO("kaa's eye", sFxKaasEye,   FXPERIOD, FXDURATION),
    FXLOOP_INFO("noise1",    sFxNoise1,    FXPERIOD, FXDURATION),
    FXLOOP_INFO("noise2",    sFxNoise2,    FXPERIOD, FXDURATION),
    FXLOOP_INFO("huenoise1", sFxHueNoise1, FXPERIOD, FXDURATION),
    FXLOOP_INFO("huefill",   sFxHueFill,   FXPERIOD, FXDURATION),

    // TODO: stars, hsvsweep, strobo, waves, ...
};


// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // initialise random number generator
    hwMathSeedRandom(hwGetRandomSeed());


#if 0
    for (U y = 0; y < FF_LEDFX_NUM_Y; y++)
    {
        for (U x = 0; x < FF_LEDFX_NUM_X; x++)
        {
            ledfxSetMatrixHSV(x, y, 0, 255, 255);
        }
    }
    while (ENDLESS)
    {
        osTaskDelay(100);
    }
#endif

#if 0
    ledfxSetMatrixRGB(0, 0, 255, 0, 0);
    ledfxSetMatrixRGB(3, 1, 0, 255, 0);
    ledfxSetMatrixRGB(2, 2, 0, 0, 255);
    ledfxSetMatrixRGB(6, 3, 0, 255, 255); // xxx
    ledfxSetMatrixRGB(2, 4, 255, 0, 255);

    ledfxSetMatrixRGB(6, 5, 255, 255, 255);
    ledfxSetMatrixRGB(6, 6, 255, 255, 255);
    sLedFlush();
    while (ENDLESS)
    {
        osTaskDelay(100);
    }
#endif

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    ledfxSetBrightness(200);

    while (ENDLESS)
    {
        const U2 res = fxloopRun(false);

        // update matrix?
        if (res == FLUSH_MATRIX)
        {
            sLedFlush();
        }

        // delay until it's time to run the next iteration of the effect
        fxloopWait();
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("%"F_U2"mA "), sAppCurrent);
    str[size-1] = '\0';
}


//@}
// eof

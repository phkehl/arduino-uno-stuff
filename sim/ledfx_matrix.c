/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED FX Simulator

    - Copyright (c) 2014-2018 Philippe Kehl (flipflip at oinkzwurgl dot org)
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdarg.h>        // libc: variable argument lists
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: standard library definitions
#include <time.h>          // libc: time types
#include <sys/time.h>      // libc: time types
#include <unistd.h>        // libc: standard symbolic constants and types
#include <assert.h>        // libc: verify program assertions
#include <signal.h>        // libc: signals

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "stdstuff.h"      // ff: useful macros and types
#include "sdl.h"           // ff: sdl stuff
#include "sim.h"           // ff: simulator mocks
#include "hsv2rgb.h"       // ff: HSV to RGB conversion
#include "ledfx.h"         // ff: LED effects
#include "fxloop.h"        // ff: effects loops


/* ***** matrix emulation **************************************************** */

static uint32_t sHsvaToColour(const uint8_t h, const uint8_t s, const uint8_t v, const uint8_t a)
{
    uint8_t r = 0, g = 0, b = 0;
    hsv2rgb(h, s, v, &r, &g, &b);
    return SDL_FOURCC(r, g, b, a);
}
static bool sCfgDrawGrid = true;
static int sCfgSpeed = 50;
static bool sCfgForceNext = false;
static int sCfgLedStyle = 0;

enum { LED_ROUND, LED_SQUARE, _NUMSTYLES = 2 };

static void sHandleEvent(const SDL_Event *pkEvent)
{
    switch (pkEvent->type)
    {
        // key press
        case SDL_KEYDOWN:
            switch (pkEvent->key.keysym.sym)
            {
                case SDLK_l:
                    sCfgLedStyle++;
                    sCfgLedStyle %= _NUMSTYLES;
                    break;
                case SDLK_g:
                    sCfgDrawGrid = !sCfgDrawGrid;
                    break;
                case SDLK_SPACE:
                    sCfgForceNext = true;
                    break;
                case SDLK_PAGEUP:
                    sCfgSpeed += 10;
                    if (sCfgSpeed > 100) { sCfgSpeed = 100; }
                    break;
                case SDLK_PAGEDOWN:
                    sCfgSpeed -= 10;
                    if (sCfgSpeed < 0) { sCfgSpeed = 0; }
                    break;
                case SDLK_UP:
                    sCfgSpeed++;
                    if (sCfgSpeed > 100) { sCfgSpeed = 100; }
                    break;
                case SDLK_DOWN:
                    sCfgSpeed--;
                    if (sCfgSpeed < 0) { sCfgSpeed = 0; }
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

static void sDrawCanvas(SDL_Renderer *pRenderer)
{
    int wCanvas, hCanvas;
    if (SDL_GetRendererOutputSize(pRenderer, &wCanvas, &hCanvas) != 0)
    {
        return;
    }

    // calculate position and size of LEDs
    const int wMatrix = FF_LEDFX_NUM_X;
    const int hMatrix = FF_LEDFX_NUM_Y;
    const double rCanvas = (double)wCanvas / (double)hCanvas;
    const double rMatrix = (double)wMatrix / (double)hMatrix;
    const int whGrid = (((rCanvas > rMatrix) ? ((hCanvas - 10) / hMatrix) : ((wCanvas - 10) / wMatrix)) / 2) * 2;
    const int whGrid2 = whGrid / 2;
    const int pGrid = 2;
    const int rLed = whGrid2 - pGrid;
    const int dLed = 2 * rLed;
    const int yOff = (hCanvas - (hMatrix * whGrid)) / 2;
    const int xOff = (wCanvas - (wMatrix * whGrid)) / 2;
    const int xMax = (wMatrix * whGrid) + xOff;
    const int yMax = (hMatrix * whGrid) + yOff;

    //DEBUG("renderer %p canvas=%dx%d=%.1f matrix=%dx%d=%.1f grid=%d led=%d xyOff=%d/%d xyMax=%d/%d", pRenderer,
    //    wCanvas, hCanvas, rCanvas, wMatrix, hMatrix, rMatrix, whGrid, rLed, xOff, yOff, xMax, yMax);

    // draw grid
    if (sCfgDrawGrid)
    {
        const uint32_t cGrid = sHsvaToColour(0, 0, 100, SDL_ALPHA_OPAQUE);
        for (int x = 0; x <= wMatrix; x++)
        {
            const int xLine = xOff + (x * whGrid);
            thickLineColor(pRenderer, xLine, yOff, xLine, yMax, 3, cGrid);
        }
        for (int y = 0; y <= hMatrix; y++)
        {
            const int yLine = yOff + (y * whGrid);
            thickLineColor(pRenderer, xOff, yLine, xMax, yLine, 3, cGrid);
        }
    }

    // draw LEDs
    {
        //const uint32_t cLed = sHsvaToColour(0, 0, 20, SDL_ALPHA_OPAQUE);
        for (int x = 0; x < wMatrix; x++)
        {
            for (int y = 0; y < hMatrix; y++)
            {
                uint8_t r = 0, g = 0, b = 0;
                ledfxGetMatrixRGB(x, FF_LEDFX_NUM_Y - 1 - y, &r, &g, &b);
                const uint32_t cLed = SDL_FOURCC(r, g, b, SDL_ALPHA_OPAQUE);
                switch (sCfgLedStyle)
                {
                    case LED_ROUND:
                    {
                        const int xLed = xOff + (x * whGrid) + whGrid2;
                        const int yLed = yOff + (y * whGrid) + whGrid2;
                        filledCircleColor(pRenderer, xLed, yLed, rLed, cLed);
                        break;
                    }
                    case LED_SQUARE:
                    {
                        const int xLed = xOff + (x * whGrid) + pGrid;
                        const int yLed = yOff + (y * whGrid) + pGrid;
                        boxColor(pRenderer, xLed, yLed, xLed + dLed, yLed + dLed, cLed);
                        break;
                    }
                }
            }
        }
    }
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
    LEDFX_RANDFILL_t randfill;
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

static uint16_t sFxHueSweep(const uint16_t frame)
{
   ledfxHueSweep(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
   return FLUSH_MATRIX;
}

static uint16_t sFxStars(const uint16_t frame)
{
    ledfxStars(frame == 0 ? true : false, sFxState.stars, NUMOF(sFxState.stars));
    return FLUSH_MATRIX;
}

static uint16_t sFxStrobo(const uint16_t frame)
{
   ledfxStrobo(frame == 0 ? true : false, 0, 0, &sFxState.u[0]);
   return FLUSH_MATRIX;
}

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

static uint16_t sFxRandFill(const uint16_t frame)
{
    ledfxRandFill(frame == 0 ? true : false, &sFxState.randfill);
    return FLUSH_MATRIX;
}

#define FXDURATION (uint32_t)120000

// the effects
static const FXLOOP_INFO_t skFxloops[] PROGMEM =
{
  //FXLOOP_INFO("noise 1",    sFxNoise1,    10, 100, FXDURATION),
    FXLOOP_INFO("noise 2",    sFxNoise2,    10, 100, FXDURATION),
    FXLOOP_INFO("rotor",      sFxRotor,     10, 250, FXDURATION), // frame 7-10ms
    FXLOOP_INFO("plasma",     sFxPlasma,    50, 250, FXDURATION), // frame ~50ms
    FXLOOP_INFO("kaa's eye",  sFxKaasEye,   15, 150, FXDURATION),
    FXLOOP_INFO("rain",       sFxRain,      30, 250, FXDURATION),
    FXLOOP_INFO("rainbow",    sFxRainbow,   15, 250, FXDURATION),
    FXLOOP_INFO("stars",      sFxStars,      5, 250, FXDURATION),
    FXLOOP_INFO("hue noise",  sFxHueNoise,  30, 250, FXDURATION),
    FXLOOP_INFO("waves",      sFxWaves,     50, 250, FXDURATION),
    FXLOOP_INFO("randfill",   sFxRandFill,  10, 250, FXDURATION),
    FXLOOP_INFO("diagonal",   sFxDiagonal,  10, 250, FXDURATION),
    FXLOOP_INFO("strobo",     sFxStrobo,    10, 250, FXDURATION),
    FXLOOP_INFO("huesweep",   sFxHueSweep,  10, 250, FXDURATION),
};

#define TRANSTIME 750

/* ***** application "task" ************************************************** */

#define WINDOW_TITLE "flipflipmatrix "STRINGIFY(FF_LEDFX_NUM_X)"x"STRINGIFY(FF_LEDFX_NUM_Y)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    simInit();

    NOTICE(WINDOW_TITLE);

    if (!sdlInit(WINDOW_TITLE, sDrawCanvas, sHandleEvent))
    {
        ERROR("Ouch!");
        return EXIT_FAILURE;
    }

    //ledfxClear(0, 0);
    //ledfxSetMatrixRGB(0, 0, 255, 0, 0);
    //ledfxSetMatrixRGB(1, 1, 0, 255, 0);
    //ledfxSetMatrixRGB(2, 2, 0, 0, 255);
    //sdlHandle();
    //sdlUpdate();
    //osTaskDelay(10000);

    // initialise effects loop
    fxloopInit(skFxloops, NUMOF(skFxloops), true);

    PRINT("Keys: [UP]/[DN]/[PGUP]/[PGDN] = speed, [SPACE] = next fx, [l] = LED style, [q] = quit");

    while (sdlHandle())
    {
        // render next frame
        const uint16_t res = fxloopRun(sCfgForceNext);
        sCfgForceNext = false;

        const uint8_t currentFx = fxloopCurrentlyPlaying();
        sdlSetInfoNw("fx %u/%d: %s (%u)",
            currentFx, (int)NUMOF(skFxloops), skFxloops[currentFx - 1].fxName, fxloopCurrentlyRemaining());
        sdlSetInfoNe("style %i, speed %i", sCfgLedStyle, sCfgSpeed);

        // update matrix?
        if (res == FLUSH_MATRIX)
        {
            sdlUpdate();
        }

        fxloopWait((uint8_t)sCfgSpeed);
    }

#if 0
    static float r0;
    ledfxPlasma(true, &r0);
    static uint32_t frame;

    while (sdlHandle())
    {
        frame++;
        ledfxClear(0, 0);
        ledfxPlasma(false, &r0);
        sdlSetInfoNw("#%06u", frame);
        sdlSetInfoNe("%g", r0);
        sdlUpdate();
        osTaskDelay(100);
    }
#endif

    sdlShutdown();

    return EXIT_SUCCESS;
}

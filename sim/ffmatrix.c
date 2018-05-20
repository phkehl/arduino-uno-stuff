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


static uint32_t sHsvaToColour(const uint8_t h, const uint8_t s, const uint8_t v, const uint8_t a)
{
    uint8_t r = 0, g = 0, b = 0;
    hsv2rgb(h, s, v, &r, &g, &b);
    return SDL_FOURCC(r, g, b, a);
}
static bool sCfgDrawGrid = true;

static void sHandleEvent(const SDL_Event *pkEvent)
{
    switch (pkEvent->type)
    {
        // key press
        case SDL_KEYDOWN:
            switch (pkEvent->key.keysym.sym)
            {
                case SDLK_g:
                    sCfgDrawGrid = !sCfgDrawGrid;
                    break;
                case SDLK_PLUS:
                case SDLK_UP:
                    sdlSetFramerate(sdlGetFramerate() + 1);
                    break;
                case SDLK_MINUS:
                case SDLK_DOWN:
                    sdlSetFramerate(sdlGetFramerate() - 1);
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
    const int rLed = whGrid2 - 2;
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
                const int xLed = xOff + (x * whGrid) + whGrid2;
                const int yLed = yOff + (y * whGrid) + whGrid2;
                uint8_t r = 0, g = 0, b = 0;
                ledfxGetMatrixRGB(x, FF_LEDFX_NUM_Y - 1 - y, &r, &g, &b);
                const uint32_t cLed = SDL_FOURCC(r, g, b, SDL_ALPHA_OPAQUE);
                filledCircleColor(pRenderer, xLed, yLed, rLed, cLed);
            }
        }
    }
}

#define WINDOW_TITLE "flipflipmatrix "STRINGIFY(FF_LEDFX_NUM_X)"x"STRINGIFY(FF_LEDFX_NUM_Y)

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
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

    static float r0;
    ledfxPlasma(true, &r0);

    while (sdlHandle())
    {
        ledfxClear(0, 0);
        ledfxPlasma(false, &r0);

        sdlUpdate();
    }


    sdlShutdown();

    return EXIT_SUCCESS;
}

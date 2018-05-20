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


static void sDrawCanvas(SDL_Renderer *pRenderer)
{
    int rWidth, rHeight;
    if (SDL_GetRendererOutputSize(pRenderer, &rWidth, &rHeight) != 0)
    {
        return;
    }
    //DEBUG("renderer %p %dx%d", pRenderer, rWidth, rHeight);
    filledCircleRGBA(pRenderer, 100, 100, 50, 100, 0, 0, SDL_ALPHA_OPAQUE);
    filledCircleRGBA(pRenderer, 120, 140, 50, 0, 100, 0, 100);
}


int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    NOTICE("Hoihoi!");

    if (!sdlInit("flipflipmatrix "STRINGIFY(FF_LEDFX_NUM_X)"x"STRINGIFY(FF_LEDFX_NUM_Y), sDrawCanvas))
    {
        ERROR("Ouch!");
        return EXIT_FAILURE;
    }

    while (sdlHandle())
    {
        sdlUpdate();
    }


    sdlShutdown();

    return EXIT_SUCCESS;
}

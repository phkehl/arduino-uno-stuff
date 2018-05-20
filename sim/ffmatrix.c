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
#include <SDL2/SDL2_framerate.h>

#include "stdstuff.h"      // ff: useful macros and types

#include "sdl.h"           // ff: sdl stuff
#include "sim.h"           // ff: simulator mocks


int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);
    NOTICE("Hoihoi!");

    if (!sdlInit())
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

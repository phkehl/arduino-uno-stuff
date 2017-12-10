/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED FX Simulator

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)
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

#include <SDL/SDL.h>

#include "stdstuff.h"      // ff: useful macros and types
#include "sim.h"

int main(int argc, char **argv)
{
    UNUSED(argc);
    UNUSED(argv);

    fprintf(stderr, "hoihoi\n");

    ERROR("foo!");

    return(EXIT_SUCCESS);
}

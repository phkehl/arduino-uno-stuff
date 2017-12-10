/*!
    \file
    \brief flipflip's Arduino Uno stuff: simulator mocks

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdarg.h>        // libc: variable argument lists
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions
#include <stdlib.h>        // libc: general utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "sim.h"           // ff: simulator mocks

uint32_t hwMathGetRandom(void)
{
    return random();
}

float hwMathFastCosf(const float phi)
{
    return cosf(phi);
}

float hwMathFastSinf(const float phi)
{
    return sinf(phi);
}

float hwMathFastSqrtf(const float x)
{
    return sqrtf(x);
}

void hwMathSeedRandom(const uint32_t seed)
{
    srandom(seed);
}

void hwTic(const uint8_t reg)
{
    UNUSED(reg);
}

uint16_t hwToc(const uint8_t reg)
{
    UNUSED(reg);
    return 0;
}


// eof

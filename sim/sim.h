/*!
    \file
    \brief flipflip's Arduino Uno stuff: simulator mocks

    - Copyright (c) 2017-2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#ifndef __SIM_H__
#define __SIM_H__

#include "stdstuff.h"      // ff: useful macros and types

/* *************************************************************************** */

// debug.h
#define NOTICE(fmt, args...)    fprintf(stderr, "\e[1mN: "  fmt "\e[m\n", ## args)
#define PRINT(fmt, args...)     fprintf(stderr, "\e[mP: "   fmt "\e[m\n", ## args)
#define WARNING(fmt, args...)   fprintf(stderr, "\e[33mW: " fmt "\e[m\n", ## args)
#define ERROR(fmt, args...)     fprintf(stderr, "\e[31mE: " fmt "\e[m\n", ## args)
#define DEBUG(fmt, args...)     fprintf(stderr, "\e[36mD: " fmt "\e[m\n", ## args)
#define NOTICE_W(fmt, args...)  NOTICE(fmt, ## args)
#define PRINT_W(fmt, args...)   PRINT(fmt, ## args)
#define WARNING_W(fmt, args...) WARNING(fmt, ## args)
#define ERROR_W(fmt, args...)   ERROR(fmt, ## args)
#define DEBUG_W(fmt, args...)   DEBUG(fmt, ## args)

// hw.h
uint32_t hwMathGetRandom(void);
float hwMathFastCosf(const float phi);
float hwMathFastSinf(const float phi);
float hwMathFastSqrtf(const float x);
void hwMathSeedRandom(const uint32_t seed);
void hwTic(const uint8_t reg);
uint16_t hwToc(const uint8_t reg);


/* *************************************************************************** */

#endif // __SIM_H__

//@}
// eof

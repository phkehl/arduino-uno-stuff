/*!
    \file
    \brief flipflip's Arduino Uno stuff: simulator mocks

    - Copyright (c) 2017-2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#ifndef __SIM_H__
#define __SIM_H__

#include "stdstuff.h"      // ff: useful macros and types

/* *************************************************************************** */

void simInit(void);


// avr
#define PROGMEM /* nothing */
#define PSTR(x) x
#define snprintf_P snprintf
#define pgm_read_byte(x) (*(x))
#define pgm_read_word(x) (*(x))
#define pgm_read_dword(x) (*(x))

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

// os.h
void osTaskDelay(const uint32_t timeout);
bool osTaskDelayUntil(uint32_t *pPrevTick, const uint32_t incrTicks);
uint32_t osTaskGetMsss(void);
uint32_t osTaskGetTicks(void);

/* *************************************************************************** */

#endif // __SIM_H__

//@}
// eof

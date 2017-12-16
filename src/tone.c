/*!
    \file
    \brief flipflip's Arduino Uno stuff: tone and melody generator (see \ref TONE)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup TONE
    @{
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "tone.h"          // ff: tone and melody generator

/* ************************************************************************** */

#if (FF_TONE_ENABLE > 0)

void toneInit(void)
{
    DEBUG("tone: init pin "STRINGIFY(FF_TONE_PIN));
    PIN_OUTPUT(FF_TONE_PIN);
    PIN_LOW(FF_TONE_PIN);
}

void toneGenerate(const uint16_t freq, const uint16_t dur)
{
    UNUSED(freq);
    UNUSED(dur);
}



#endif // (FF_TONE_ENABLE > 0)

/* ************************************************************************** */

//@}
// eof

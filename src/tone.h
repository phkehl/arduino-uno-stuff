/*!
    \file
    \brief flipflip's Arduino Uno stuff: tone and melody generator (see \ref TONE)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup TONE Tone and Melody Generator
    \ingroup FF

    This implements a tone and melody generator. It uses Timer/Counter2 (TCCR2) while tone(s) are
    output.

    Configuration:
    - #FF_TONE_ENABLE  must be set to 1 to enable this
    - #FF_TONE_PIN     pin to use

    See \ref EXAMPLES_EX11 for a demonstration.

    @{
*/

#ifndef __TONE_H__
#define __TONE_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise tone and melody generator
void toneInit(void);

//! generate a tone
/*!
    Generates a tone of given frequency for the given duration.
    Does not block while the tone is generated.

    \param[in] freq  the frequency [Hz]
    \param[in] dur   the duration [ms]
*/
void toneGenerate(const uint16_t freq, const uint16_t dur);


/* *************************************************************************** */

#endif // __TONE_H__

//@}
// eof

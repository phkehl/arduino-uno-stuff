/*!
    \file
    \brief flipflip's Arduino Uno stuff: Adeunis ARF32 bluetooth module (see \ref PROJECTS_FFPHONE_ARF32)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFPHONE_ARF32 ARF32
    \ingroup PROJECTS_FFPHONE

    This module implements configuration and control of the Adeunis (http://www.adeunis-rf.com/en)
    RF ARF32 bluetooth module, which is based on the National Semiconductor LMX9830 bluetooth chip
    and the Winbond W681360 voice codec chip.

    - [ARF32 User Guide](../../projects/ffphone/arf32_user_guide.pdf)
    - [ARF32 Datasheet](../../projects/ffphone/arf32_datasheet.pdf)
    - [ARF32 Audio User Guide](../../projects/ffphone/arf32_audio_user_guide.pdf)
    - [ARF32 Audio Quick Start](../../projects/ffphone/arf32_audio_quick.pdf)
    - [W681360 Audio Codec](../../projects/ffphone/w681360_voice_codec.pdf)
    - see also \ref PROJECTS_FFPHONE_LMX9830 for those datasheets

    @{
*/

#ifndef __ARF32_H__
#define __ARF32_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

void arf32Init(void);

void arf32Start(void);

void arf32Status(char *str, const size_t size);


//! state of the ARF32 module
typedef enum ARF32_STATE_e
{
    ARF32_STATE_UNKNOWN, //!< unknown/undefined stated
    ARF32_STATE_READY,   //!< ready, waiting for commands
    ARF32_STATE_PAIRED,  //!< ready, paired, waiting for commands or calls
    ARF32_STATE_INCALL,  //!< in call
    ARF32_STATE_ERROR,   //!< bad error occured
} ARF32_STATE_t;


/* *************************************************************************** */

#endif // __ARF32_H__
// @}
// eof

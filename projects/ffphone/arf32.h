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

    @{
*/

#ifndef __ARF32_H__
#define __ARF32_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

void arf32Start(void);

void arf32Status(char *str, const size_t size);



/* *************************************************************************** */

#endif // __ARF32_H__
// @}
// eof

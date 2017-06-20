/*!
    \file
    \brief flipflip's Arduino Uno stuff: Silvertel Ag1170 Low Power Ringing SLIC module (see \ref PROJECTS_FFPHONE_AG1170)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFPHONE_AG1170 AG1170
    \ingroup PROJECTS_FFPHONE

    Stuff for the Silvertel Ag1170 Low Power Ringing SLIC (subscriber line interface circuit).

    - [AG1170 Datasheet](../../projects/ffphone/ag1170_datasheet.pdf)

    @{
*/

#ifndef __AG1170_H__
#define __AG1170_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

void ag1170Init(void);

void ag1170Start(void);

void ag1170Status(char *str, const size_t size);


//! state of the AG1170 module
typedef enum AG1170_STATE_e
{
    AG1170_STATE_UNKNOWN, //!< unknown/undefined stated
    AG1170_STATE_READY,   //!< ready
} AG1170_STATE_t;



/* *************************************************************************** */

#endif // __AG1170_H__
// @}
// eof

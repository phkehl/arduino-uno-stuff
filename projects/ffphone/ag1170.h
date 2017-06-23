/*!
    \file
    \brief flipflip's Arduino Uno stuff: Silvertel Ag1170 Low Power Ringing SLIC module (see \ref PROJECTS_FFPHONE_AG1170)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFPHONE_AG1170 AG1170
    \ingroup PROJECTS_FFPHONE

    Stuff for the Silvertel Ag1170 Low Power Ringing SLIC (subscriber line interface circuit).

    - [AG1170 Datasheet](../../projects/ffphone/ag1170_datasheet.pdf)

    Four pins are used to interface to the following AG1170 module functions:
    - power down (PD) the module (the DCDC converter) when high
    - ring mode (RM) enables the ringing voltage generator (output) when high
    - forward/reverse (F/R) reverse the ringing voltage polarity when low
    - switch hook (SHK) tells us whether the receiver is on-hook (low) of off-hook (high)

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
    // keep in sync with skStateStrs
    AG1170_STATE_UNKNOWN, //!< unknown/undefined stated
    AG1170_STATE_ERROR,   //!< bad error occured
    AG1170_STATE_ONHOOK,  //!< on-hook
    AG1170_STATE_OFFHOOK, //!< off-hook
} AG1170_STATE_t;

AG1170_STATE_t ag1170GetState(void);

//! stringify state
/*!
    \param[in] state  the state to stringify
    \returns the string representation of the current state
*/
const char *ag1170StateStr(const AG1170_STATE_t state);


/* *************************************************************************** */

#endif // __AG1170_H__
// @}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: Siemens DL2416T (see \ref PROJECTS_FFLEDTESTER_DL2416T)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFLEDTESTER_DL2416T Siemens DL2416T
    \ingroup PROJECTS_FFLEDTESTER

    @{
*/

#ifndef __DL2416T_H__
#define __DL2416T_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise hardware
void dl2416tInit(void);

//! write character to position
void dl2416tWrite(const uint8_t pos, const char ch);

//! clear display
void dl2416tClear(void);

//! blank/unblank display (without loosing contents)
void dl2416tBlank(const bool blank);

//! blink display
void dl2416tBlink(const uint8_t times, const uint32_t tOff, const uint32_t tOn);

//! put PROGMEM string to display
void dl2416tStr_P(const char *str, const uint8_t offs, const uint8_t nChars);

//! scroll PROGMEM string over display
void dl2416tStrScroll_P(const char *str, const uint32_t delay);

//! display unsigned number
void dl2416tUnsigned(const uint16_t n, const uint8_t offs, const uint8_t nDigits);


/* *************************************************************************** */

#endif // __DL2416T_H__
// @}
// eof

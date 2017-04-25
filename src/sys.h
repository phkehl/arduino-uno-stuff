/*!
    \file
    \brief flipflip's Arduino Uno stuff: system task (see \ref SYS)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \defgroup SYS System Task
    \ingroup FF

    This implements the system task.

    @{
*/

#ifndef __SYS_H__
#define __SYS_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

void sysInit(void);

void sysCreateSystemTask(void);

// return a pointer suitable for the initial stack pointer
uint16_t sysGetInitStackPointer(void);


/* *************************************************************************** */

//! user monitor function type
typedef void (* SYS_MON_FUNC_t)(char *, size_t);

//! register user monitor function
/*!
    \param[in] func  function that creates the string to output in the monitor task
*/
void sysRegisterMonFunc(SYS_MON_FUNC_t func);


/* *************************************************************************** */

#endif // __SYS_H__

//@}
// eof

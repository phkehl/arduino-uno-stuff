/*!
    \file
    \brief flipflip's Arduino Uno stuff: system task (see \ref SYS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup SYS System Task
    \ingroup FF

    This implements the system task.

    Configuration:
    - #FF_SYS_MON_PERIOD
    - #FF_SYS_MON_VERBOSE
    - #FF_SYS_TASK_PRIO
    - #FF_SYS_TASK_STACK
    - #FF_SYS_SW_WATCHDOG

    @{
*/

#ifndef __SYS_H__
#define __SYS_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

void sysInit(void);

// create system task
void sysCreateSystemTask(void);

// return a pointer suitable for the initial stack pointer
uint16_t sysGetInitStackPointer(void);


/* *************************************************************************** */

//! user monitor function type, see sysRegisterMonFunc()
typedef void (* SYS_MON_FUNC_t)(char *, size_t);

//! register user monitor function
/*!
    \param[in] func  function that creates the string to output in the monitor task
*/
void sysRegisterMonFunc(SYS_MON_FUNC_t func);

//! assert software watchdog
/*!
    Asserts the software watchdog. Active only if compiled in by setting #FF_SYS_SW_WATCHDOG > 0.
*/
void sysAssertSwWatchdog(void);


/* *************************************************************************** */

#endif // __SYS_H__

//@}
// eof

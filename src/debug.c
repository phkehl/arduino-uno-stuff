/*!
    \file
    \brief flipflip's Arduino Uno stuff: debugging stuff (see \ref DEBUG)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup DEBUG
    @{
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdarg.h>        // libc: variable argument lists
#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions

#include <avr/pgmspace.h>  // avr-libc: program space utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware


OS_MUTEX_t sDebugLockMutex;

void debugInit(void)
{
    osMutexCreate(&sDebugLockMutex);
}

void debugLock(void)
{
    if (osTaskIsSchedulerRunning())
    {
        osMutexClaim(&sDebugLockMutex, 0);
    }
}

void debugUnlock(void)
{
    if (osTaskIsSchedulerRunning())
    {
        osMutexRelease(&sDebugLockMutex);
    }
}


//@}
// eof

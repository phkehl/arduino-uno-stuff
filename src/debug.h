/*!
    \file
    \brief flipflip's Arduino Uno stuff: debugging stuff (see \ref DEBUG)

    - Copyright (c) 2016-2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup DEBUG Debugging
    \ingroup FF

    Debugging output helpers. Note that these must not be used in interrupts!

    Configuration:
    - #FF_DEBUG_LEVEL

    @{
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__


/* *************************************************************************** */

#include <stdio.h>         // libc: standard buffered input/output
#ifdef __AVR__
#  include <avr/pgmspace.h>// avr-libc: program Space Utilities
#else
#  define PSTR(foo) foo
#  define printf_P(args...) fprintf(stderr, ## args)
#endif

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "hw.h"            // ff: hardware
#include "os.h"            // ff: operating system


// initialises the debugging subsystem
void debugInit(void);


// locking of PRINT() (et al.)
//#define DEBUG_LOCK   /* nothing */
//#define DEBUG_UNLOCK /* nothing */
//#define DEBUG_LOCK   CS_ENTER
//#define DEBUG_UNLOCK CS_LEAVE
void debugLock(void);
void debugUnlock(void);
#define DEBUG_LOCK   debugLock()
#define DEBUG_UNLOCK debugUnlock()


//! prints a notice
/*!
    Prints the formatted notice message, automatically
    prepended with "N: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define NOTICE(fmt, args...) \
    DEBUG_LOCK; printf_P(PSTR("N: " fmt "\n"), ## args); DEBUG_UNLOCK


//! prints a print
/*!
    Prints the formatted print message, automatically
    prepended with "N: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define PRINT(fmt, args...) \
    DEBUG_LOCK; printf_P(PSTR("P: " fmt "\n"), ## args); DEBUG_UNLOCK


//! prints a warning
/*!
    Prints the formatted warning message, automatically
    prepended with "W: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define WARNING(fmt, args...) \
    DEBUG_LOCK; printf_P(PSTR("W: " fmt "\n"), ## args); DEBUG_UNLOCK


//! prints an error
/*!
    Prints the formatted error message, automatically
    prepended with "E: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define ERROR(fmt, args...) \
    DEBUG_LOCK; printf_P(PSTR("E: " fmt "\n"), ## args); DEBUG_UNLOCK


//! prints a debug message
/*!
    Prints the formatted debug message (if #FF_DEBUG_LEVEL >= 1),
    automatically prepended with "D: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#if ( (FF_DEBUG_LEVEL > 0) || defined(__DOXYGEN__) )
#  define DEBUG(fmt, args...) \
    DEBUG_LOCK; printf_P(PSTR("D: " fmt "\n"), ## args); DEBUG_UNLOCK
#else
#  define DEBUG(...) /* nothing */
#endif


//! like NOTICE() but will wait for empty tx buffer \hideinitializer
#define NOTICE_W(fmt, args...)  hwTxWaitEmpty(); NOTICE(fmt, ## args)

//! like PRINT() but will wait for empty tx buffer \hideinitializer
#define PRINT_W(fmt, args...)   hwTxWaitEmpty(); PRINT(fmt, ## args)

//! like WARNING() but will wait for empty tx buffer \hideinitializer
#define WARNING_W(fmt, args...) hwTxWaitEmpty(); WARNING(fmt, ## args)

//! like ERROR() but will wait for empty tx buffer \hideinitializer
#define ERROR_W(fmt, args...)   hwTxWaitEmpty(); ERROR(fmt, ## args)

//! like DEBUG() but will wait for empty tx buffer \hideinitializer
#define DEBUG_W(fmt, args...)   hwTxWaitEmpty(); DEBUG(fmt, ## args)


/* *************************************************************************** */

#endif // __DEBUG_H__

//@}
// eof

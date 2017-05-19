/*!
    \file
    \brief flipflip's Arduino Uno stuff: debugging stuff (see \ref DEBUG)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup DEBUG Debugging
    \ingroup FF

    Debugging output helpers.

    @{
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__


/* *************************************************************************** */

#include <stdio.h>         // libc: standard buffered input/output
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "hw.h"            // ff: hardware
#include "os.h"            // ff: operating system


// initialises the debugging subsystem
void debugInit(void);


// debug message level (type, prefix)
typedef enum DEBUG_LEVEL_e
{
    DEBUG_LEVEL_ERROR   = 1, // "E: "
    DEBUG_LEVEL_WARNING = 2, // "W: "
    DEBUG_LEVEL_NOTICE  = 3, // "N: "
    DEBUG_LEVEL_PRINT   = 4, // "P: "
    DEBUG_LEVEL_DEBUG   = 5  // "D: "
} DEBUG_LEVEL_t;

// prints some frequently used string constants
void debugConsts(const DEBUG_LEVEL_t k);


//! prints a notice
/*!
    Prints the formatted notice message, automatically
    prepended with "N: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define NOTICE(fmt, args...) \
    /*CS_ENTER; */debugConsts(DEBUG_LEVEL_NOTICE); printf_P(PSTR(fmt "\n"), ## args)/*; CS_LEAVE*/


//! prints a print
/*!
    Prints the formatted print message, automatically
    prepended with "N: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define PRINT(fmt, args...) \
    /*CS_ENTER; */debugConsts(DEBUG_LEVEL_PRINT); printf_P(PSTR(fmt "\n"), ## args)/*; CS_LEAVE*/


//! prints a warning
/*!
    Prints the formatted warning message, automatically
    prepended with "W: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define WARNING(fmt, args...) \
    /*CS_ENTER; */debugConsts(DEBUG_LEVEL_WARNING); printf_P(PSTR(fmt "\n"), ## args)/*; CS_LEAVE*/


//! prints an error
/*!
    Prints the formatted error message, automatically
    prepended with "E: ".

    \param fmt   the format
    \param args  zero or more arguments

    \hideinitializer
*/
#define ERROR(fmt, args...) \
    /*CS_ENTER; */debugConsts(DEBUG_LEVEL_ERROR); printf_P(PSTR(fmt "\n"), ## args)/*; CS_LEAVE*/


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
    /*CS_ENTER; */debugConsts(DEBUG_LEVEL_DEBUG); printf_P(PSTR(fmt "\n"), ## args)/*; CS_LEAVE*/
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

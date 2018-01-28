/*!
    \file
    \brief flipflip's Arduino Uno stuff: first example application (see \ref EXAMPLES_EX01)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup EXAMPLES_EX01 1st Example Application
    \ingroup EXAMPLES

    This is the first example application. It demonstrates some basic \ref OS and \ref ARDUINOPINS
    functionality.

    See ex01.c source code for details.

    @{
*/

#ifndef __EX01_H__
#define __EX01_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __EX01_H__
// @}
// eof

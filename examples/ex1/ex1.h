/*!
    \file
    \brief flipflip's Arduino Uno stuff: first example application (see \ref EXAMPLES_EX1)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \defgroup EXAMPLES_EX1 1st Example Application
    \ingroup EXAMPLES

    This is the first example application. It demonstrates some basic \ref OS and \ref UNOPINS
    functionality.

    See ex1.c source code for details.

    @{
*/

#ifndef __EX1_H__
#define __EX1_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __EX1_H__
// @}
// eof

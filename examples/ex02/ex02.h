/*!
    \file
    \brief flipflip's Arduino Uno stuff: second example application (see \ref EXAMPLES_EX02)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup EXAMPLES_EX02 2nd Example Application
    \ingroup EXAMPLES

    This is the second example application. It demonstrates some basic \ref OS and \ref UNOPINS
    functionality as well as some \ref HW configuration.

    See ex02.c source code for details.

    @{
*/

#ifndef __EX02_H__
#define __EX02_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __EX02_H__
// @}
// eof

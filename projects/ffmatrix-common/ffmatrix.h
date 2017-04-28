/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix common stuff (see \ref PROJECTS_FFMATRIX_COMMON)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFMATRIX_COMMON flipflip matrix common code
    \ingroup PROJECTS

    <img src="../../projects/ffmatrix-common/schematics.png"/>

    - [schematics.pdf](../../projects/ffmatrix-common/schematics.pdf)

    See ffmatrix.c source code and https://oinkzwurgl.org/projaeggd/flipflipmatrix for details.

    \todo port Linux LED matrix simulator from old repo/code

    @{
*/

#ifndef __FFMATRIX_H__
#define __FFMATRIX_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFMATRIX_H__
// @}
// eof

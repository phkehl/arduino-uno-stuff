/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFMATRIX7X7 flipflip matrix 7x7 RGB LED Matrix
    \ingroup PROJECTS


    <img src="../../projects/ffmatrix7x7/ffmatrix7x7.jpg"/>

    <img src="../../projects/ffmatrix7x7/top.jpg" width="50%"/>

    <img src="../../projects/ffmatrix7x7/bottom.jpg" width="50%"/>

    \todo This is the wrong schema. But's it's close.. :-)
    <img src="../../projects/ffmatrix7x7/schematics.png"/>
    [Schematics](../../projects/ffmatrix7x7/schematics.pdf)

    See ffmatrix7x7.c source code and https://oinkzwurgl.org/projaeggd/flipflipmatrix for details.

    @{
*/

#ifndef __FFMATRIX7X7_H__
#define __FFMATRIX7X7_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFMATRIX7X7_H__
// @}
// eof

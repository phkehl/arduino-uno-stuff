/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 8x8 (see \ref PROJECTS_FFMATRIX8X8)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFMATRIX8X8 flipflip matrix 8x8 RGB LED Matrix
    \ingroup PROJECTS


    Made from some cheap RGB LED matrix from aliexpress.com using \ref ALIMATRIX with
    #FF_ALIMATRIX_MODE = 2.

    <img src="../../projects/ffmatrix8x8/ffmatrix8x8-1.jpg"/>
    <img src="../../projects/ffmatrix8x8/ffmatrix8x8-2.jpg"/>



    See \ref PROJECTS_FFMATRIX_COMMON and https://oinkzwurgl.org/projaeggd/flipflipmatrix for details.

    @{
*/

#ifndef __FFMATRIX8X8_H__
#define __FFMATRIX8X8_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFMATRIX8X8_H__
//@}
// eof

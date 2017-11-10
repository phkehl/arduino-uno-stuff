/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's funky clock (see \ref PROJECTS_FFFUNKYCLOCK)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFFUNKYCLOCK flifplip clock
    \ingroup PROJECTS

    <img src="../../projects/fffunkyclock/fffunkyclock.jpg"/>

    See also \ref PROJECTS_FFCLOCK.

    See fffunkyclock.c source code and https://oinkzwurgl.org/projaeggd/flipflipclock for details.

    @{
*/

#ifndef __FFFUNKYCLOCK_H__
#define __FFFUNKYCLOCK_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFFUNKYCLOCK_H__
// @}
// eof

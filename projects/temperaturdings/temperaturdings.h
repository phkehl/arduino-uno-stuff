/*!
    \file
    \brief flipflip's Arduino Uno stuff: Temperatur Dings (see \ref PROJECTS_TEMPERATURDINGS)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_TEMPERATURDINGS Temperatur Dings
    \ingroup PROJECTS

    A temperature and humidity sensor using \ref SI7021 and \ref SSD1306.

    See temperaturdings.c source code for details.

    @{
*/

#ifndef __TEMPERATURDINGS_H__
#define __TEMPERATURDINGS_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __TEMPERATURDINGS_H__
// @}
// eof

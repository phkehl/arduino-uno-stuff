/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFCLOCK flifplip clock
    \ingroup PROJECTS

    <img src="../../projects/ffclock/ffclock.jpg"/>
    <img src="../../projects/ffclock/ffclock-annot.jpg" width="50%"/>

    - [28BYJ-48.pdf](../../projects/ffclock/docs/28BYJ-48.pdf) (stepper motor)
    - [dialight_547-2xxx.pdf](../../projects/ffclock/docs/dialight_547-2xxx.pdf) (LED modules)
    - [helical_antenna.pdf](../../projects/ffclock/docs/helical_antenna.pdf) (from http://home.iae.nl/users/plundahl/antenne/helical.htm, https://web.archive.org/web/20091210100330/http://home.iae.nl/users/plundahl/antenne/helical.htm)
    - [LN2003A.pdf](../../projects/ffclock/docs/LN2003A.pdf) (darlington transistor array)
    - [L293.pdf](../../projects/ffclock/docs/L293.pdf) (driver)
    - [panasonic_av4444.pdf](../../projects/ffclock/docs/panasonic_av4444.pdf) (mini switches)
    - [NEO-M8_DataSheet_(UBX-13003366).pdf](../../projects/ffclock/docs/NEO-M8_DataSheet_(UBX-13003366).pdf) (GNSS receiver)
    - [NEO-M8_HardwareIntegrationManual_(UBX-13003557).pdf](../../projects/ffclock/docs/NEO-M8_HardwareIntegrationManual_(UBX-13003557).pdf) (GNSS receiver)
    - [NEO-M8_ProductSummary_(UBX-13003449).pdf](../../projects/ffclock/docs/NEO-M8_ProductSummary_(UBX-13003449).pdf) (GNSS receiver)

    See ffclock.c source code and https://oinkzwurgl.org/projaeggd/flipflipclock for details.

    @{
*/

#ifndef __FFCLOCK_H__
#define __FFCLOCK_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFCLOCK_H__
// @}
// eof

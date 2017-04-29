/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFCLOCK
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


//  LED for the system load and status
#define FF_HW_LOAD_PIN _PD4

//! system tick signal
#define FF_HW_TICK_PIN _PD5

//! serial port input buffer size
#define FF_HW_RX_BUFSIZE 125


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: Machki application (see \ref PROJECTS_MASCHKI)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_MASCHKI
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


// use the built-in LED for the system load and status
#define FF_HW_LOAD_PIN _D13

// gain some memory by disabling serial port receive
#define FF_HW_RX_BUFSIZE 0


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

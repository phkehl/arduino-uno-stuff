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


// gain some memory by disabling serial port receive
#define FF_HW_RX_BUFSIZE 0

// we have this many LEDs
#define FF_LEDFX_NUM_LED 2

// GRB colour ordering
#define FF_LEDFX_ORDER 123

// precise time measurement
#define FF_HW_NUM_TICTOC 1

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

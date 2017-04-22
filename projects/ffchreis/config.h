/*!
    \file
    \brief flipflip's Arduino Uno stuff: "ffchräis" LED rings application (see \ref PROJECTS_FFCHREIS)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup PROJECTS_FFCHREIS
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


// use the built-in LED for the system load and status
#define FF_HW_LOAD_PIN 13

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// loose some memory by using a slightly smother hsv2rgb() conversion method
#define FF_HSV2RGB_METHOD 2

// we have this many LEDs
#define FF_LEDFX_NUM_LED (1+8+12+16+24)

// GRB colour ordering
#define FF_LEDFX_ORDER 213


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

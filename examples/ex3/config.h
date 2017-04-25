/*!
    \file
    \brief flipflip's Arduino Uno stuff: third example application (see \ref EXAMPLES_EX3)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX3
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */

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

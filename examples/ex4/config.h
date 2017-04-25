/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourth example application (see \ref EXAMPLES_EX4)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX4
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */


// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// we have this many LEDs
#define FF_LEDFX_NUM_LED 2

// GRB colour ordering
#define FF_LEDFX_ORDER 123

//#define FF_HSV2RGB_METHOD 2

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

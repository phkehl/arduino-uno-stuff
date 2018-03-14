/*!
    \file
    \brief flipflip's Arduino Uno stuff: fourth example application (see \ref EXAMPLES_EX04)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX04
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_HW_LOAD_PIN        _D9
#define FF_HW_TICK_PIN        _NONE

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// we have this many LEDs
#define FF_LEDFX_NUM_LED 10

// BGR colour ordering
#define FF_LEDFX_ORDER 321

//#define FF_HSV2RGB_METHOD 2

#define FF_SK9822_SPI_SPEED 500


//#define FF_HSV2RGB_METHOD 2

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

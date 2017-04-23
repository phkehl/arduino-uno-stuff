/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2016-2017 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup PROJECTS_FFMATRIX7X7
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


// LED for the system load and status
#define FF_HW_LOAD_PIN PD4

// system tick signal
#define FF_HW_TICK_PIN PD3

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// loose some memory by using a slightly smother hsv2rgb() conversion method
#define FF_HSV2RGB_METHOD 2

// matrix dimension
#define FF_LEDFX_NUM_X 7
#define FF_LEDFX_NUM_Y 7

// we have this many LEDs
#define FF_LEDFX_NUM_LED ((FF_LEDFX_NUM_X) * (FF_LEDFX_NUM_Y))

// LEd matrix arrangement
#define FF_LEDFX_XY_ARR 1

// BRG colour ordering
#define FF_LEDFX_ORDER 312

// SPI master speed
#define FF_WS2801_SPI_SPEED 1000


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

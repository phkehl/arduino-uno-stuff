/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED Matrix Simulator

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


// matrix dimension
#define FF_LEDFX_NUM_X 8
#define FF_LEDFX_NUM_Y 8

// we have this many LEDs
#define FF_LEDFX_NUM_LED ((FF_LEDFX_NUM_X) * (FF_LEDFX_NUM_Y))

// LED matrix arrangement
#define FF_LEDFX_XY_ARR 4


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

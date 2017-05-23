/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip matrix 7x7 (see \ref PROJECTS_FFMATRIX7X7)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFMATRIX7X7
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */

// ../common/ffmatrix.c configuration
#define FFMATRIX_MODEL      88
#define FFMATRIX_DRIVER     2           // LED driver (1 = WS2801, 2 = Alimatrix)
#define FFMATRIX_FLUSH_LED  _D4         // LED lit while data is written to the LED strip
#define FFMATRIX_SPEED_POT  HW_ADC_A5   // potentiometer to change speed

// system load LED
#define FF_HW_LOAD_PIN _D3

// idle thread stack size
#define FF_OS_IDLE_STACK 85

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// matrix dimension
#define FF_LEDFX_NUM_X 8
#define FF_LEDFX_NUM_Y 8

// we have this many LEDs
#define FF_LEDFX_NUM_LED ((FF_LEDFX_NUM_X) * (FF_LEDFX_NUM_Y))

// LED matrix arrangement
#define FF_LEDFX_XY_ARR 4

// BRG colour ordering
//#define FF_LEDFX_ORDER 123

// use brightness levels
#define FF_ALIMATRIX_MODE 2

// system monitor period
#define FF_SYS_MON_PERIOD 1


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

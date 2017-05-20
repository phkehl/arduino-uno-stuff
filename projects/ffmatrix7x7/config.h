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
#define FFMATRIX_MODEL      77
#define FFMATRIX_DRIVER     1           // LED driver (1 = WS2801, 2 = Alimatrix)
#define FFMATRIX_FLUSH_LED  _PC5        // LED lit while data is written to the LED strip
#define FFMATRIX_SPEED_POT  HW_ADC_PC3  // potentiometer to change speed
#define FFMATRIX_BRIGHT_POT HW_ADC_PC4  // potentiometer to change brightness
#define FFMATRIX_MA_PER_LED 60          // [mA] per LED if fully lit
#define FFMATRIX_PSU_MAX_MA 2500        // maximum [mA] the power supply can deliver
#define FFMATRIX_MODE_PIN_0 _PD5        // mode display 1st pin (LED), LSB
#define FFMATRIX_MODE_PIN_1 _PD6        // mode display 2nd pin (LED)
#define FFMATRIX_MODE_PIN_2 _PD7        // mode display 3rd pin (LED)
#define FFMATRIX_MODE_PIN_3 _PB0        // mode display 4th pin (LED), MSB


// idle thread stack size
#define FF_OS_IDLE_STACK 85

// LED for the system load and status
#define FF_HW_LOAD_PIN _PD4

// system tick signal
#define FF_HW_TICK_PIN _PD3

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// loose some memory by using a slightly smother hsv2rgb() conversion method
#define FF_HSV2RGB_METHOD 2

// matrix dimension
#define FF_LEDFX_NUM_X 7
#define FF_LEDFX_NUM_Y 7

// we have this many LEDs
#define FF_LEDFX_NUM_LED ((FF_LEDFX_NUM_X) * (FF_LEDFX_NUM_Y))

// LED matrix arrangement
#define FF_LEDFX_XY_ARR 1

// BRG colour ordering
#define FF_LEDFX_ORDER 312

// SPI master speed
#define FF_WS2801_SPI_SPEED 1000

// system monitor period
#define FF_SYS_MON_PERIOD 1


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: Temperatur Dings (see \ref PROJECTS_TEMPERATURDINGS)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_TEMPERATURDINGS
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */

// use the built-in LED for the system load and status
#define FF_HW_LOAD_PIN _D13

// allow more debugging output
#define FF_HW_TX_BUFSIZE  250
#define FF_SYS_MON_STRLEN 128
#define FF_SYS_TASK_STACK 350

// gain some memory by disabling serial port receive1
#define FF_HW_RX_BUFSIZE 0

// Wemos D1 Mini OLED 1.0
#define FF_SSD1306_DISPLAY_SIZE 6448
#define FF_SSD1306_DISPLAY_ADDR 0x3c

// we can go faster than the default
#define FF_I2C_MASTER_SPEED 400


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: HD44780 LCD Driver (see \ref HD44780)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup HD44780 HD44780 LCD Driver
    \ingroup FF

    This implements a HD44780 LCD driver.

    - [[HD44780](../HD44780.pdf)] Hitachi HD44780U (LCD-II) Dot Matrix Liquid Crystal Display Controller/Driver
    - [[ST7066](../ST7066_HD44780.pdf)] Sitronix ST7066 Dot Matrix LCD Controller/Driver
    - [[PCF8574](../PCF8574.pdf)] NXP PCF8574 Remote 8-bit I/O Expander for I2C-bus With Interrupt
    - https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller

    Configuration:
    - #FF_HD44780_CONN
    - #FF_HD44780_PCF8574_I2C_ADDR
    - #FF_HD44780_LINES
    - #FF_HD44780_COLUMNS

    See \ref EXAMPLES_EX10 for an example.

    @{
*/

#ifndef __HD44780_H__
#define __HD44780_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise the HD44780 LCD driver
void hd44780Init(void);


/* *************************************************************************** */

#endif // __HD44780_H__

//@}
// eof

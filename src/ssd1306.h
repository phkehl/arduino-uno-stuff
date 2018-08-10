/*!
    \file
    \brief flipflip's Arduino Uno stuff: SSD1306 SSD1306 monochrome OLED display driver (see \ref SSD1306)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup SSD1306 SSD1305 OLED display driver
    \ingroup FF

    This implements a driver for the Solomon Systech SSD1306 Dit Matrix OLED/PLED Segment/Common
    Driver with Controller chip. It uses the \ref I2C driver.

    pins:

    - pin A4 is SDA (serial data)
    - pin A5 is SCL (serial clock)

    See \ref EXAMPLES_EX15 for a demonstration.

    References:
    - [SI7021](../SSD1306.pdf) Solomon Systech SSD1306 Datasheet

    @{
*/

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things
void ssd1306Init(void);

/* *************************************************************************** */

#endif // __SSD1306_H__

//@}
// eof

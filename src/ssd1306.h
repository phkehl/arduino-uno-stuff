/*!
    \file
    \brief flipflip's Arduino Uno stuff: SSD1306 SSD1306 monochrome OLED display driver (see \ref SSD1306)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    Based on Adafruit's SSD1306 library for the Arduine (https://github.com/adafruit/Adafruit_SSD1306)

    - Copyright (c) Limor Fried/Ladyada, Adafruit Industries.

    \defgroup SSD1306 SSD1305 display driver
    \ingroup FF

    This implements a driver for the Solomon Systech SSD1306 Dit Matrix OLED/PLED Segment/Common
    Driver with Controller chip. It uses the \ref I2C driver, which uses the following pins:
    - pin A4 is SDA (serial data)
    - pin A5 is SCL (serial clock)

    Configuration:
    - #FF_SSD1306_DISPLAY_SIZE
    - #FF_SSD1306_DISPLAY_ADDR

    See \ref EXAMPLES_EX15 for a demonstration.

    References:
    - [SI7021](../SSD1306.pdf) Solomon Systech SSD1306 Datasheet

    @{
*/

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "stdstuff.h"      // ff: useful macros and types
#include "gfx.h"           // ff: graphics primitives


/* *************************************************************************** */

//! initialise things
void ssd1306Init(void);

//! physical width of the display
/*!
    \returns the physical width of the display in [px]
*/
uint16_t ssd1306Width(void);

//! physical height of the display
/*!
    \returns the physical height of the display in [px]
*/
uint16_t ssd1306Height(void);

//! set one pixel in frame buffer
/*!
    Coordinates are zero based. The origin (x/y = 0/0) is the top left corner of the display. X is
    horizontal (right), y vertical (down).

    \param[in] x       x coordinate of the pixel
    \param[in] y       y coordinate of the pixel
    \param[in] colour  colour to change the pixel to
*/
void ssd1306Pixel(uint16_t x, uint16_t y, GFX_COLOUR_t colour);

//! clear display frame buffer
void ssd1306Clear(void);

//! write frame buffer to display
void ssd1306Update(void);

//! invert display
void ssd1306Invert(const bool invert);

//! dim display
void ssd1306Dim(const bool dim);


/* *************************************************************************** */

#endif // __SSD1306_H__

//@}
// eof

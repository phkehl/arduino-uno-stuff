/*!
    \file
    \brief flipflip's Arduino Uno stuff: HD44780 LCD Driver (see \ref HD44780)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup HD44780 HD44780 LCD Driver
    \ingroup FF

    This implements a HD44780 LCD driver. It uses a modified version of Andrei Ostanin's avr-hd44780
    library (see lcd.c) for the low-level interface.

    - [[HD44780](../HD44780.pdf)] Hitachi HD44780U (LCD-II) Dot Matrix Liquid Crystal Display Controller/Driver
    - [[ST7066](../ST7066_HD44780.pdf)] Sitronix ST7066 Dot Matrix LCD Controller/Driver

    - [[PCF8574](../PCF8574.pdf)] NXP PCF8574 Remote 8-bit I/O Expander for I2C-bus With Interrupt
    - https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller

    Configuration:
    - #FF_HD44780_LINES
    - #FF_HD44780_COLUMNS

    See \ref EXAMPLES_EX10 for an example.

    \todo character mapping from PC (latin1?) to HD44780 charset

    @{
*/

#ifndef __HD44780_H__
#define __HD44780_H__

#include <stdarg.h>        // libc: variable argument list

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise the HD44780 LCD driverm, call once
void hd44780Init(void);

//! clear display and put cursor at (0, 0)
void hd44780Clear(void);

//! put cursor at given position
/*!
    \param[in] line   line number (0 = first line)
    \param[in] column column number (0 = first column)
*/
void hd44780PutCursor(const uint8_t line, const uint8_t column);

//! set cursor mode
/*!
    \param[in] enable  enable (\c true) or disable (\c false) the cursor
    \param[in] blink   blinking (c true) or static (\c false) cursor,
                       only used with \c enable = \c true
*/
void hd44780CursorMode(const bool enable, const bool blink);

//! write character at current cursor position and advance cursor
/*!
    \param[in] c  the character
*/
void hd44780Write(const char c);

//! print formatted string at cursor position and advance cursor, like printf_P()
/*!
    \param[in] fmt  the format string
    \param[in] ...  format string arguments
*/
void hd44780Printf_P(const char *fmt, ...);

//! create custom character
/*!
    \param[in] c       character code to program
    \param[in] bitmap  the 5x8 pixels bitmap for the character (in #PROGMEM)

    The \c bitmap works as follows:
\code{.c}
static const uint8_t skHeart[8] PROGMEM = // 5x8
{
    0b00000, // .....
    0b01010, // .#.#.
    0b11111, // #####
    0b11111, // #####
    0b01110, // .###.
    0b00100, // ..#..
    0b00000, // .....
};
\endcode
*/
void hd44780CreateChar(const char c, const uint8_t bitmap[8]);


/* *************************************************************************** */

#endif // __HD44780_H__

//@}
// eof

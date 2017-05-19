/*!
    \file
    \brief flipflip's Arduino Uno stuff: Aliexpress LED matrix driver (see \ref ALIMATRIX)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup ALIMATRIX Aliexpress LED Matrix Driver
    \ingroup FF

    This implements a driver for some Aliexpress LED matrix based on the 74HC595 chip. It uses the following pins:

    - pin 13 is the clock output (SCK), connect to CLK (clock in) of the LED matrix module
    - pin 11 is the data output (MOSI), connect to MOSI (data in) of the LED matrix module
    - pin 10 is the slave select signal (SS), connect to CE (chip enable?) of the LED matrix module

    See \ref EXAMPLES_EX7 and \ref EXAMPLES_EX8 for a demonstrations.

    References:

    - [SN74HC595](../SN74HC595.pdf) 8-Bit Shift Register With 3-State Output Registers

    <img src="../../doc/alimatrix1.jpg">
    <img src="../../doc/alimatrix2.jpg">


    @{
*/

#ifndef __ALIMATRIX_H__
#define __ALIMATRIX_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things (the SPI master interface)
void alimatrixInit(void);

//! start driving matrix
void alimatrixStart(void);

/*!
    \name simple operation mode functions

    Set #FF_ALIMATRIX_MODE to 1 to use these.

    @{
*/

//! clear matrix
/*!
    \note Only available if #FF_ALIMATRIX_MODE is 1.
*/
void alimatrixClear(void);

//! set pixel
/*!
    \note Only available if #FF_ALIMATRIX_MODE is 1.

    \param[in] x      x coordinate
    \param[in] y      y coordinate
    \param[in] red    turn on red LED at coordinate
    \param[in] green  turn on green LED at coordinate
    \param[in] blue   turn on blue LED at coordinate
*/
void alimatrixSetXY(const uint8_t x, const uint8_t y, const bool red, const bool green, const bool blue);

//! set whole row
/*!
    \note Only available if #FF_ALIMATRIX_MODE is 1.

    \param[in] row    row number
    \param[in] red    bitmap for red LEDs in row
    \param[in] green  bitmap for green LEDs in row
    \param[in] blue   bitmap for blue LEDs in row
*/
void alimatrixSetRow(const uint8_t row, const uint8_t red, const uint8_t green, const uint8_t blue);

//@}

/* *************************************************************************** */

/*!
    \name extend operation mode functions

    Set #FF_ALIMATRIX_MODE to 2 to use this.

    @{
*/

//! update matrix framebuffer
/*!
    This assumes that \c data is formatted suitably for the 8x8 matrix (i.e. it's 8 x 8 x 3 = 192
    bytes in size).

    \param[in] data  data
*/
void alimatrixUpdate(const uint8_t *data);

//@}

/* *************************************************************************** */

#endif // __ALIMATRIX_H__

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: WS2801 LED driver (see \ref WS2801)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup WS2801 WS2801 LED Driver
    \ingroup FF

    This implements a WS2801 LED driver. It uses the following pins:

    - pin 13 is the clock output (SCK), connect to CI (clock in) of the first WS2801 LED
    - pin 12 (MISO) isn't used (but blocked)
    - pin 11 is the data output (MOSI), connect to DI (data in) of the first WS2801 LED
    - pin 10 is the slave select signal (SS)

    Note that the SS signal isn't used by the WS2801 chip. However, the AVR's SPI peripheral seems
    to require that the CS pin is configured as an output in order for the SPI master mode to
    work. Hence this driver drives that signal (active low), which might be useful for debugging.
    Nor are we reading anything on the MISO pin, but when SPI master mode is active this pin is
    unavailable.

    Configuration:
    - #FF_WS2801_SPI_SPEED

    See \ref EXAMPLES_EX04 for a demonstration.

    References:
    - [WS2801](../WS2801.pdf) Worldsemi WS2801 Datasheet

    @{
*/

#ifndef __WS2801_H__
#define __WS2801_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things (the SPI master interface)
void ws2801Init(void);

//! send data to the WS2891 chip(s)
/*!
    \param[in] data  data
    \param[in] size  data size
*/
void ws2801Send(const uint8_t *data, const uint16_t size);


/* *************************************************************************** */

#endif // __WS2801_H__

//@}
// eof

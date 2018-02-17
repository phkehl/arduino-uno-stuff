/*!
    \file
    \brief flipflip's Arduino Uno stuff: WS2812 LED driver (see \ref WS2812)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup WS2812 WS2812 LED Driver
    \ingroup FF

    This implements a WS2812 LED driver based on a modified version of the "light_ws2812" library
    (by Time a.k.a. cpldcpu, https://github.com/cpldcpu/light_ws2812).

    References:

    - [WS2812](../WS2812.pdf) Worldsemi WS2812 Datasheet
    - [WS2812B](../WS2812B.pdf) Worldsemi WS2812B Datasheet
    - https://github.com/cpldcpu/light_ws2812
    - https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/

    See \ref EXAMPLES_EX03 for a demonstration.

    @{
*/

#ifndef __WS2812_H__
#define __WS2812_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! send data to WS2812 chip(s)
/*!
    \note This will block the system and system ticks may be skipped.

    \param[in] pin   pin number for the data signal
    \param[in] data  data (const uint8_t *), NULL to blank
    \param[in] size  data size (uint8_t)

    \hideinitializer
*/
#define ws2812Send(pin, data, size) \
    _ws2812Send(&PIN_TO_PORT(pin), &PIN_TO_DDR(pin), PIN_TO_MASK(pin), data, size)

void _ws2812Send(volatile uint8_t *pPort, volatile uint8_t *pDdr, uint8_t mask, const uint8_t *data, const uint8_t size);



/* *************************************************************************** */

#endif // __WS2812_H__

//@}
// eof

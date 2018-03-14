/*!
    \file
    \brief flipflip's Arduino Uno stuff: SK9822 (and APA102) LED driver (see \ref SK9822)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup SK9822 SK9822 / APA102 LED Driver
    \ingroup FF

    This implements a SK9822 LED driver. It uses the following pins:

    - pin 13 is the clock output (SCK), connect to CI (clock in) of the first SK9822 LED
    - pin 12 (MISO) isn't used (but blocked)
    - pin 11 is the data output (MOSI), connect to DI (data in) of the first SK9822 LED
    - pin 10 is the slave select signal (SS)

    Note that the SS signal isn't used by the SK9822 chip. However, the AVR's SPI peripheral seems
    to require that the CS pin is configured as an output in order for the SPI master mode to
    work. Hence this driver drives that signal (active low), which might be useful for debugging.
    Nor are we reading anything on the MISO pin, but when SPI master mode is active this pin is
    unavailable.

    This driver implements a unified protocol that works with SK9822 and APA102 LEDs as outlined by
    Tim in his blog (https://cpldcpu.wordpress.com/2016/12/13/sk9822-a-clone-of-the-apa102/)

    Configuration:
    - #FF_SK9822_SPI_SPEED

    See \ref EXAMPLES_EX13 for a demonstration.

    References:
    - [SK9822](../SK9822.pdf) Shenzen Normand Electronic Co., Ltd. SK9822 Datasheet
    - [APA102](../APA102.pdf) APA Electronic Co., Ltd. APA102 Datasheet (2013)
    - [APA102C](../APA102C.pdf) APA Electronic Co., Ltd. APA102C Datasheet (2014)

    @{
*/

#ifndef __SK9822_H__
#define __SK9822_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise things (the SPI master interface)
void sk9822Init(void);

//! send data to the SK9822 chip(s)
/*!
    \param[in] data  data (3 bytes per LED, BGR order), NULL to blank
    \param[in] size  data size

    This will use the global brightness level set by sk9822SetBrightness().
*/
void sk9822Send(const uint8_t *data, const uint16_t size);

//! set global brightness
/*!
    \param[in] brightness  global brightness level (0..31), 0 = dark (off)
*/
void sk9822SetBrightness(const uint8_t brightness);

//! send raw data to the SK9822 chip(s)
/*!
    \param[in] data  data (4 bytes per LED, brightness + 3 bytes in BGR order), NULL to blank
    \param[in] size  data size
*/
void sk9822SendRaw(const uint8_t *data, const uint16_t size);



/* *************************************************************************** */

#endif // __SK9822_H__

//@}
// eof

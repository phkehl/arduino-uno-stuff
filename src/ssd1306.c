/*!
    \file
    \brief flipflip's Arduino Uno stuff: SSD1306 SSD1306 monochrome OLED display driver (see \ref SSD1306)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup SSD1306
    @{

*/

#include <string.h>        // libc: string operations

#include <util/twi.h>      // avr-libc: TWI bit mask definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "i2c.h"           // ff: i2c driver
#include "ssd1306.h"       // ff: SSD1306 driver


/* ************************************************************************** */

void ssd1306Init(void)
{
    i2cInit();
    DEBUG("ssd1306: init");
}



/* ************************************************************************** */

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: HD44780 LCD Driver (see \ref HD44780)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup HD44780 HD44780 LCD Driver
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/pgmspace.h>  // avr: program space utilities

#include <lcd.h>           // Andrei's avr-hd44780 library

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "hd44780.h"       // ff: HD44780 LCD driver


/* ************************************************************************** */

void hd44780Init(void)
{
    DEBUG("hd44780: init");

    lcd_init();
    lcd_on();

    hd44780Clear();
}

__INLINE void hd44780Clear(void)
{
    lcd_clear();
    lcd_return_home();
}

__INLINE void hd44780PutCursor(const uint8_t line, const uint8_t column)
{
    lcd_set_cursor(column, line);
}

__INLINE void hd44780Write(const char c)
{
    uint8_t v = (uint8_t)c;
    lcd_write(v);
}

static char sBuf[FF_HD44780_COLUMNS + 1];

void hd44780Printf_P(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf_P(sBuf, sizeof(sBuf), fmt, args);
    va_end(args);
    char *pBuf = sBuf;
    while (*pBuf != '\0')
    {
        hd44780Write(*pBuf);
        pBuf++;
    }
}

void hd44780CreateChar(const char c, const uint8_t bitmap[8])
{
    uint8_t location = (uint8_t)c;
    uint8_t charmap[8];
    for (uint8_t ix = 0; ix < sizeof(charmap); ix++)
    {
        charmap[ix] = pgm_read_byte(&bitmap[ix]);
    }
    lcd_create_char(location, charmap);
}

void hd44780CursorMode(const bool enable, const bool blink)
{
    if (enable)
    {
        lcd_enable_cursor();
        if (blink)
        {
            lcd_enable_blinking();
        }
        else
        {
            lcd_disable_blinking();
        }
    }
    else
    {
        lcd_disable_cursor();
    }
}

/* ************************************************************************** */

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: HD44780 LCD Driver (see \ref HD44780)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup HD44780 HD44780 LCD Driver
    @{
*/

#include <string.h>        // libc: string operations

#include "lcd.h"           // Andrei's avr-hd44780 library

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

void hd44780Clear(void)
{
    lcd_clear();
    lcd_return_home();
}

void hd44780Cursor(const uint8_t column, const uint8_t line)
{
    lcd_set_cursor(column, line);
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
        lcd_write(*pBuf);
        pBuf++;
    }
}


/* ************************************************************************** */

//@}
// eof

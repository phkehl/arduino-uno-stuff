/*!
    \file
    \brief flipflip's Arduino Uno stuff: Aliexpress LED matrix driver (see \ref ALIMATRIX)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup ALIMATRIX
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: interrupt things

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "alimatrix.h"     // ff: Aliexpress LED matrix driver


/* ************************************************************************** */

void alimatrixInit(void)
{
    // configure hardware SPI
    PIN_OUTPUT(_D13); // SCK
    PIN_OUTPUT(_D11); // MOSI
    PIN_OUTPUT(_D10); // SS
    PIN_HIGH(_D10);

    // enable, master mode, f/128 (125kHz)
    SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1) | BIT(SPR0);
    SPSR = 0;

    DEBUG("alimatrix: init");
}


enum { RED = 0, GREEN = 2, BLUE = 1 };

static uint8_t fb[8][3];/* =
{
    { 0x18, 0x00, 0x00 },
    { 0x3c, 0x00, 0x00 },
    { 0x7e, 0x00, 0x00 },
    { 0xff, 0x00, 0x00 },
    { 0xff, 0x00, 0x00 },
    { 0xff, 0x00, 0x00 },
    { 0x66, 0x00, 0x00 },
    { 0x00, 0x00, 0x00 },
    };*/

void alimatrixClear(void)
{
    memset(fb, 0, sizeof(fb));
}

void alimatrixSetXY(const uint8_t x, const uint8_t y, const bool red, const bool green, const bool blue)
{
    if ( (x < 8) && (y < 8) )
    {
        const uint8_t row = y;
        const uint8_t bit = BIT(7 - x);
        if (red)   { SETBITS(fb[row][RED],   bit); } else { CLRBITS(fb[row][RED],   bit); }
        if (green) { SETBITS(fb[row][GREEN], bit); } else { CLRBITS(fb[row][GREEN], bit); }
        if (blue)  { SETBITS(fb[row][BLUE],  bit); } else { CLRBITS(fb[row][BLUE],  bit); }
    }
}

void alimatrixSetRow(const uint8_t row, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    if (row < 8)
    {
        fb[row][RED]   = red;
        fb[row][GREEN] = green;
        fb[row][BLUE]  = blue;
    }
}

void alimatrixStart(void)
{
    SPCR |= BIT(SPIE);
    SPDR = 0;
}

static volatile uint8_t svRow;
static volatile uint8_t svByte;

ISR(SPI_STC_vect) // SPI, serial transfer complete
{
    osIsrEnter();

    if (svByte == 0)
    {
        PIN_HIGH(_D10);
        svRow++;
        svRow %= 8;

        PIN_LOW(_D10);
        SPDR = ~fb[svRow][0];
        svByte++;
    }
    else if (svByte == 1)
    {
        SPDR = ~fb[svRow][1];
        svByte++;
    }
    else if (svByte == 2)
    {
        SPDR = ~fb[svRow][2];
        svByte++;
    }
    else
    {
        SPDR = BIT(svRow);
        svByte = 0;
    }

    osIsrLeave();
}


/* ************************************************************************** */

//@}
// eof

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

#if (FF_ALIMATRIX_MODE != 1) && (FF_ALIMATRIX_MODE != 2) && (FF_ALIMATRIX_MODE != 3)
#  error illegal value for FF_ALIMATRIX_MODE
#endif


/* ************************************************************************** */

void alimatrixInit(void)
{
    // configure hardware SPI
    PIN_OUTPUT(_D13); // SCK
    PIN_OUTPUT(_D11); // MOSI
    PIN_OUTPUT(_D10); // SS
    PIN_HIGH(_D10);

    // enable, master mode, f/128 (125kHz)
#if (FF_ALIMATRIX_MODE == 1)
    SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1) | BIT(SPR0);
    SPSR = 0;
#endif

    // enable, master mode, f/64 (250kHz)
#if (FF_ALIMATRIX_MODE == 2) || (FF_ALIMATRIX_MODE == 3)
    SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1);
    SPSR = 0;
#endif

    DEBUG("alimatrix: init");
}

void alimatrixStart(void)
{
    SPCR |= BIT(SPIE);
    SPDR = 0;
}


/* ************************************************************************** */
#if (FF_ALIMATRIX_MODE == 1)

enum { RED = 0, GREEN = 2, BLUE = 1 };

static volatile uint8_t svFb[8][3];

void alimatrixClear(void)
{
    memset(svFb, 0, sizeof(svFb));
}

void alimatrixSetXY(const uint8_t x, const uint8_t y, const bool red, const bool green, const bool blue)
{
    if ( (x < 8) && (y < 8) )
    {
        const uint8_t row = y;
        const uint8_t bit = BIT(7 - x);
        if (red)   { SETBITS(svFb[row][RED],   bit); } else { CLRBITS(svFb[row][RED],   bit); }
        if (green) { SETBITS(svFb[row][GREEN], bit); } else { CLRBITS(svFb[row][GREEN], bit); }
        if (blue)  { SETBITS(svFb[row][BLUE],  bit); } else { CLRBITS(svFb[row][BLUE],  bit); }
    }
}

void alimatrixSetRow(const uint8_t row, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    if (row < 8)
    {
        svFb[row][RED]   = red;
        svFb[row][GREEN] = green;
        svFb[row][BLUE]  = blue;
    }
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
        SPDR = ~svFb[svRow][0];
        svByte++;
    }
    else if (svByte == 1)
    {
        SPDR = ~svFb[svRow][1];
        svByte++;
    }
    else if (svByte == 2)
    {
        SPDR = ~svFb[svRow][2];
        svByte++;
    }
    else
    {
        SPDR = BIT(svRow);
        svByte = 0;
    }

    osIsrLeave();
}

#endif // (FF_ALIMATRIX_MODE == 1)


/* ************************************************************************** */
#if (FF_ALIMATRIX_MODE == 2) || (FF_ALIMATRIX_MODE == 3)

#  if (FF_ALIMATRIX_MODE == 2)
#    define _N 4
#  else
#    define _N 8
#  endif

// frame x rows x colours
static volatile uint8_t svFb[_N*8][3];
static uint8_t svFbTmp[_N*8][3];

void alimatrixUpdate(const uint8_t *data)
{
    memset(svFbTmp, 0, sizeof(svFbTmp));
    for (uint8_t row = 0; row < 8; row++)
    {
        const uint8_t rowOffs = row * (8 * 3);
        for (uint8_t col = 0; col < 8; col++)
        {
            const uint8_t colOffs = rowOffs + (3 * col);
            const uint8_t c1 = data[colOffs + 0];
            const uint8_t c2 = data[colOffs + 1];
            const uint8_t c3 = data[colOffs + 2];
            const uint8_t bit = BIT(col);
            for (uint8_t lay = 0; lay < _N; lay++)
            {
                const uint8_t layIx = (8 * lay) + row;
#  if (_N == 4)
                const uint8_t thrs = BIT((2 * lay) + 1) - 1;
                //const uint8_t thrs = 1 + (lay * 50);
#  elif (_N == 8)
                const uint8_t thrs = BIT(lay + 1) - 1;
#  else
#    error ouch
#  endif
                if (c1 >= thrs) { svFbTmp[layIx][0] |= bit; }
                if (c2 >= thrs) { svFbTmp[layIx][1] |= bit; }
                if (c3 >= thrs) { svFbTmp[layIx][2] |= bit; }
            }
        }
        for (uint8_t lay = 0; lay < _N; lay++)
        {
            const uint8_t layIx = (8 * lay) + row;
            svFbTmp[layIx][0] = ~svFbTmp[layIx][0];
            svFbTmp[layIx][1] = ~svFbTmp[layIx][1];
            svFbTmp[layIx][2] = ~svFbTmp[layIx][2];
        }
    }
    CS_ENTER;
    memcpy(svFb, svFbTmp, sizeof(svFb));
    CS_LEAVE;
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
        svRow %= (_N * 8);

        PIN_LOW(_D10);
        SPDR = svFb[svRow][0];
        svByte++;
    }
    else if (svByte == 1)
    {
        SPDR = svFb[svRow][1];
        svByte++;
    }
    else if (svByte == 2)
    {
        SPDR = svFb[svRow][2];
        svByte++;
    }
    else
    {
        SPDR = BIT(svRow % 8);
        svByte = 0;
    }

    osIsrLeave();
}

#endif // (FF_ALIMATRIX_MODE == 2) || (FF_ALIMATRIX_MODE == 3)

/* ************************************************************************** */
//@}
// eof

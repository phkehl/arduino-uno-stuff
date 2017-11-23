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

#if (FF_ALIMATRIX_MODE != 1) && (FF_ALIMATRIX_MODE != 2)
#  error illegal value for FF_ALIMATRIX_MODE
#endif


/* ************************************************************************** */
#if (FF_ALIMATRIX_MODE == 1)

enum { RED = 0, GREEN = 2, BLUE = 1 };

static volatile uint8_t svFb[8][3];

void alimatrixClear(void)
{
    //memset(svFb, 0, sizeof(svFb));
    CS_ENTER;
    memset((uint8_t *)svFb, 0, sizeof(svFb));
    CS_LEAVE;
    //uint16_t ix = sizeof(svFb);
    //uint8_t *pFb = (uint8_t *)svFb;
    //while (ix-- > 0)
    //{
    //    *pFb = 0;
    //    pFb++;
    //}
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

    // enable, master mode, f/64 (250kHz)
    //SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1);
    //SPSR = 0;

    DEBUG("alimatrix: init (%"PRIu8")", (uint8_t)sizeof(svFb));
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

    // reds
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
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 2)
    {
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 3)
    {
        SPDR = BIT(svRow);
        svByte++;
    }
    // blues
    else if (svByte == 4)
    {
        PIN_HIGH(_D10);
        PIN_LOW(_D10);
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 5)
    {
        SPDR = ~svFb[svRow][1];
        svByte++;
    }
    else if (svByte == 6)
    {
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 7)
    {
        SPDR = BIT(svRow);
        svByte++;
    }
    // greens
    else if (svByte == 8)
    {
        PIN_HIGH(_D10);
        PIN_LOW(_D10);
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 9)
    {
        SPDR = 0xff;
        svByte++;
    }
    else if (svByte == 10)
    {
        SPDR = ~svFb[svRow][2];
        svByte++;
    }
    else //if (svByte == 11)
    {
        SPDR = BIT(svRow);
        svByte = 0;
    }

    osIsrLeave();
}

#endif // (FF_ALIMATRIX_MODE == 1)


/* ************************************************************************** */

#if (FF_ALIMATRIX_MODE == 2)

#  define M_X 8
#  define M_Y 8
#  define M_RED   0
#  define M_GREEN 1
#  define M_BLUE  2

// input matrix is 8 px * 8 px * 3 colours
typedef struct MATRIX_s
{
    uint8_t xy[M_Y][M_X][3];
} MATRIX_t;

// Something with this cheap matrix is wrong: if one enables different colours in the same row at
// the same time some of the colours disappear. E.g. when you enable red(s) the blue(s) will
// disappear (or get very dim). Probably something to do with the different voltage drops for the
// different colours. Bad design? By design?
// To work around this we never display different colours in the same row but instead display one
// after the other. Additionally we artificially generate four shades of brightness by enabling
// the LEDs more or less often.
// All this leads to the following scheme (each line is one "scan" of a single row):
// ........ row 1 red   1
// ........   .
// ........   .
// ........ row 8 red   1
// ........ row 1 blue  1
// ........   .
// ........   .
// ........ row 8 blue  1
// ........ row 1 green 1
// ........   .
// ........   .
// ........ row 8 green 1
//       .
//       .
//       .
// ........ row 1 red   4
// ........   .
// ........   .
// ........ row 8 red   4
// ........ row 1 blue  4
// ........   .
// ........   .
// ........ row 8 blue  4
// ........ row 1 green 4
// ........   .
// ........   .
// ........ row 8 green 4
//
// i.e.: 4 shades * 3 colours * 8 rows

#  define N_SHADES  4
#  define N_COLOURS 3
#  define N_ROWS    8
#  define I_RED     0
#  define I_GREEN   1
#  define I_BLUE    2

static volatile uint8_t svRows[N_SHADES][N_COLOURS][N_ROWS];
static uint8_t sRowsTmp[N_SHADES][N_COLOURS][N_ROWS];

void alimatrixUpdate(const uint8_t *data)
{
    // we operate on a temporary copy of the buffer; otherwise we generate
    // flicker as the ISR is reading from the live copy as we go
    memset(sRowsTmp, 0, sizeof(sRowsTmp));
    const MATRIX_t *pkM = (const MATRIX_t *)data;

    // for each x/y in the original (input) matrix
    for (uint8_t y = 0; y < M_Y; y++)
    {
        const uint8_t row = y; // y in the original matrix is a row in the target matrix
        for (uint8_t x = 0; x < M_X; x++)
        {
            // x in the original matrix is a bit in the row of the target matrix
            const uint8_t colBit = BIT(x);

            // the input colour values
            const uint8_t red   = pkM->xy[y][x][M_RED];
            const uint8_t green = pkM->xy[y][x][M_GREEN];
            const uint8_t blue  = pkM->xy[y][x][M_BLUE];

            // determine which pixels should be lit at which shade
            for (uint8_t shade = 0; shade < N_SHADES; shade++)
            {
#  if (N_SHADES == 4)
                const uint8_t thrsLut[] = { 1, 75, 150, 225 };
                const uint8_t thrs = thrsLut[shade];
#  elif (N_SHADES == 5)
                const uint8_t thrsLut[] = { 1, 51, 102, 153, 204 };
                const uint8_t thrs = thrsLut[shade];
#  elif (N_SHADES == 6)
                const uint8_t thrsLut[] = { 1, 43, 85, 128, 170, 213 };
                const uint8_t thrs = thrsLut[shade];
#  elif (N_SHADES == 8)
                const uint8_t thrsLut[] = { 1, 32, 64, 96, 128, 160, 192, 224 };
                const uint8_t thrs = thrsLut[shade];
#  else
#    error illegal value for N_SHADES
#  endif
                // if the red/green/blue colour value is above the shade's theshold
                if (red   >= thrs) { sRowsTmp[shade][I_RED  ][row] |= colBit; }
                if (green >= thrs) { sRowsTmp[shade][I_GREEN][row] |= colBit; }
                if (blue  >= thrs) { sRowsTmp[shade][I_BLUE ][row] |= colBit; }
            }
        }
    }
    // invert all bits
    uint8_t *pByte = (uint8_t *)sRowsTmp;
    uint16_t n = sizeof(sRowsTmp);
    while (n != 0)
    {
        *pByte = ~(*pByte);
        pByte++;
        n--;
    }

    // copy temporary data to live data while stopping the ISR(s) only briefly
    CS_ENTER;
    //memcpy(svRows, sRowsTmp, sizeof(svRows));
    memcpy((uint8_t *)svRows, sRowsTmp, sizeof(svRows));
    //uint16_t s = sizeof(svRows);
    //uint8_t *pRows = (uint8_t *)svRows;
    //const uint8_t *pkTmp = (const uint8_t *)sRowsTmp;
    //while (s-- > 0)
    //{
    //    *pRows = *pkTmp;
    //    pRows++;
    //    pkTmp++;
    //}
    CS_LEAVE;
}

void alimatrixInit(void)
{
    // configure hardware SPI
    PIN_OUTPUT(_D13); // SCK
    PIN_OUTPUT(_D11); // MOSI
    PIN_OUTPUT(_D10); // SS
    PIN_HIGH(_D10);

    // enable, master mode, f/128 (125kHz)
    //SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1) | BIT(SPR0);
    //SPSR = 0;

    // enable, master mode, f/64 (250kHz)
    SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR1);
    SPSR = 0;

    // anything else is too fast and calls the ISR too often so that
    // there's no CPU left for the application

    DEBUG("alimatrix: init (%"PRIu8"+%"PRIu8")",
        (uint8_t)sizeof(sRowsTmp), (uint8_t)sizeof(svRows));
}


void alimatrixStart(void)
{
#  if 0
    static uint8_t shade;
    static uint8_t colour;
    static uint8_t row = 255;
    static uint8_t byte = 4;

    while (ENDLESS)
    {
        if (byte == 4)
        {
            byte = 0;
            row++;
            if (row >= N_ROWS)
            {
                row = 0;
                colour++;
            }
            if (colour >= N_COLOURS)
            {
                colour = 0;
                shade++;
            }
            if (shade >= N_SHADES)
            {
                shade = 0;
            }
        }

        uint8_t data;
        // the 1st, 2nd or 3rd byte is the bitmat of the LEDs of this colour to lit
        if (byte == colour)
        {
            data = 0x00;
        }
        // the last byte is the row to lit
        else if (byte == 3)
        {
            data = BIT(row);
        }
        // don't enable any but the current colour
        else
        {
            data = 0xff;
        }

        // latch previous 4 bytes and start outputting the next four
        if (byte == 0)
        {
            DEBUG("---");
        }

        DEBUG("row %"PRIu8" shade %"PRIu8" colour %"PRIu8" byte=%"PRIu8" data=%02"PRIx8,
            row, shade, colour, byte, data);
        osTaskDelay(100);

        byte++;
    }
#  endif

    // enable serial transfer complete interrupt
    SPCR |= BIT(SPIE);

    // send a dummy byte to get things going
    SPDR = 0x55;
}


ISR(SPI_STC_vect) // SPI, serial transfer complete
{
    osIsrEnter();

    // determine which byte we need to send, we want:
    // row 1, shade 1, colour 1  --> 4 bytes: c1 c2 c3 r1
    // ..
    // row 8, shade 1, colour 1
    // row 1, shade 1, colour 2
    // ..
    // row 8, shade 1, colour 2
    // row 1, shade 1, colour 3
    // ..
    // row 8, shade 1, colour 3
    // .
    // .
    // row 1, shade 2, colour 1
    // ..
    // row 8, shade 2, colour 1
    // row 1, shade 2, colour 2
    // ..
    // row 8, shade 2, colour 2
    // row 1, shade 2, colour 3
    // ..
    // row 8, shade 2, colour 3
    // .
    // .
    // .
    // row 8, shade 4, colour 3

    // so for one full frame we need to send
    // 8 rows * 4 shades * 3 colours * 4 bytes/per row = 384 bytes = 3072 bits
    // at 125kHz SPI speed this gives ~40 Hz refresh rate of 40 Hz (or 10Hz for the least bright shade)
    // at 250kHz SPI speed this gives ~80 Hz refresh rate of 40 Hz (or 20Hz for the least bright shade)

    static uint8_t shade;
    static uint8_t colour;
    static uint8_t row = 255; // start at row = 0 and
    static uint8_t byte = 4;  // colour = 0

    if (byte == 4)
    {
        byte = 0;
        row++;
        if (row >= N_ROWS)
        {
            row = 0;
            colour++;
        }
        if (colour >= N_COLOURS)
        {
            colour = 0;
            shade++;
        }
        if (shade >= N_SHADES)
        {
            shade = 0;
        }
    }

    uint8_t data;
    // the 1st, 2nd or 3rd byte is the bitmat of the LEDs of this colour to lit
    if (byte == colour)
    {
        data = svRows[shade][colour][row];
    }
    // the last byte is the row to lit
    else if (byte == 3)
    {
        data = BIT(row);
    }
    // don't enable any but the current colour
    else
    {
        data = 0xff;
    }

    // latch previous 4 bytes and start outputting the next four
    if (byte == 0)
    {
        PIN_HIGH(_D10);
        PIN_LOW(_D10);
    }

    // send it
    SPDR = data;

    // prepare for next
    byte++;

    osIsrLeave();
}

#endif // (FF_ALIMATRIX_MODE == 2)

/* ************************************************************************** */
//@}
// eof

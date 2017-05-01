/*!
    \file
    \brief flipflip's Arduino Uno stuff: WS2801 LED driver (see \ref WS2801)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup WS2801
    @{
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "ws2801.h"        // ff: WS2801 LED driver


/* ************************************************************************** */

void ws2801Init(void)
{
    // configure hardware SPI
    PIN_OUTPUT(_D13); // SCK
    PIN_OUTPUT(_D11); // MOSI
    PIN_OUTPUT(_D10); // SS
    PIN_HIGH(_D10);

    // enable, master mode
    SPCR = BIT(SPE) | BIT(MSTR); // | BIT(CPOL);

#if (FF_WS2801_SPI_SPEED == 125)
    SPSR = 0;          SPCR |= BIT(SPR1) | BIT(SPR0); // f/128  125kHz
#elif (FF_WS2801_SPI_SPEED == 250)
    SPSR = 0;          SPCR |= BIT(SPR1);             // f/64   250kHz
#elif (FF_WS2801_SPI_SPEED == 500)
    SPSR = BIT(SPI2X); SPCR |= BIT(SPR1);             // f/32   500kHz
#elif (FF_WS2801_SPI_SPEED == 1000)
    SPSR = 0;          SPCR |=             BIT(SPR0); // f/16   1MHz
#elif (FF_WS2801_SPI_SPEED == 2000)
    SPSR = BIT(SPI2X); SPCR |=             BIT(SPR0); // f/8    2MHz
#elif (FF_WS2801_SPI_SPEED == 4000)
    SPSR = 0;          SPCR |= 0;                     // f/4    4MHz
#elif (FF_WS2801_SPI_SPEED == 8000)
    SPSR = BIT(SPI2X); SPCR |= 0;                     // f/2    8MHz
#else
#  error Illegal value for FF_WS2801_SPI_SPEED!
#endif

    DEBUG("ws2801: %"PRIu16"kHz", (uint16_t)FF_WS2801_SPI_SPEED);
}

void ws2801Send(const uint8_t *data, const uint8_t size)
{
    CS_ENTER;
    PIN_LOW(_D10);

    uint16_t n = size;
    while (n--)
    {
        SPDR = *data++;
        loop_until_bit_is_set(SPSR, SPIF); // delay between bytes: 1.08us
        //while (!(SPSR & BIT(SPIF)));     // delay between bytes: 1.25us
        //__asm__ volatile ("nop");        // delay between bytes: 0.167us
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
        //__asm__ volatile ("nop");
    }

    // latch data
    //_delay_us(500);
    PIN_HIGH(_D10);
    CS_LEAVE;
}

/* ************************************************************************** */

//@}
// eof

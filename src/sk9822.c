/*!
    \file
    \brief flipflip's Arduino Uno stuff: SK9822 (and APA102) LED driver (see \ref SK9822)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup SK9822
    @{

    \todo use interrupts
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "sk9822.h"        // ff: SK9822 LED driver


/* ************************************************************************** */

void sk9822Init(void)
{
    // configure hardware SPI
    PIN_OUTPUT(_D13); // SCK
    PIN_OUTPUT(_D11); // MOSI
    PIN_OUTPUT(_D10); // SS
    PIN_LOW(_D10);

    // enable, master mode
    SPCR = /*BIT(SPE) |*/ BIT(MSTR); // | BIT(CPOL);

#if (FF_SK9822_SPI_SPEED == 125)
    SPSR = 0;          SPCR |= BIT(SPR1) | BIT(SPR0); // f/128  125kHz
#elif (FF_SK9822_SPI_SPEED == 250)
    SPSR = 0;          SPCR |= BIT(SPR1);             // f/64   250kHz
#elif (FF_SK9822_SPI_SPEED == 500)
    SPSR = BIT(SPI2X); SPCR |= BIT(SPR1);             // f/32   500kHz
#elif (FF_SK9822_SPI_SPEED == 1000)
    SPSR = 0;          SPCR |=             BIT(SPR0); // f/16   1MHz
#elif (FF_SK9822_SPI_SPEED == 2000)
    SPSR = BIT(SPI2X); SPCR |=             BIT(SPR0); // f/8    2MHz
#elif (FF_SK9822_SPI_SPEED == 4000)
    SPSR = 0;          SPCR |= 0;                     // f/4    4MHz
#elif (FF_SK9822_SPI_SPEED == 8000)
    SPSR = BIT(SPI2X); SPCR |= 0;                     // f/2    8MHz
#else
#  error Illegal value for FF_SK9822_SPI_SPEED!
#endif

    sk9822SetBrightness(31);

    DEBUG("sk9822: %"PRIu16"kHz", (uint16_t)FF_SK9822_SPI_SPEED);
}

static uint8_t sSk9822bBrightness;

void sk9822SetBrightness(const uint8_t brightness)
{
    sSk9822bBrightness = 0xe0 | MIN(brightness, 0x1f);
    //DEBUG("sk9822: brightness=0x%"PRIx8, sSk9822bBrightness);
}

void sk9822Send(const uint8_t *data, const uint16_t size)
{
    CS_ENTER;
    SETBITS(SPCR, BIT(SPE)); // enable SPI

    // Tim (https://cpldcpu.wordpress.com/2016/12/13/sk9822-a-clone-of-the-apa102/) says:
    // «A protocol that is compatible to both the SK9822 and the APA102 consists of the following:
    //  1. A start frame of 32 zero bits (<0x00> <0x00> <0x00> <0x00> )
    //  2. A 32 bit LED frame for each LED in the string (<0xE0+brightness> <blue> <green> <red>)
    //  3. A SK9822 reset frame of 32 zero bits (<0x00> <0x00> <0x00> <0x00> ).
    //  4. An end frame consisting of at least (n/2) bits of 0, where n is the number of LEDs in the string.»

    // 1. start frame
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);

    // 2. LED frames
    uint16_t n = size / 3;
    while (n--)
    {
        // global brightness
        //SPDR = 0xe0 | 0x1f;
        SPDR = sSk9822bBrightness;
        loop_until_bit_is_set(SPSR, SPIF);

        // 3 channels (colours)
        SPDR = data != NULL ? *data++ : 0;
        loop_until_bit_is_set(SPSR, SPIF);
        SPDR = data != NULL ? *data++ : 0;
        loop_until_bit_is_set(SPSR, SPIF);
        SPDR = data != NULL ? *data++ : 0;
        loop_until_bit_is_set(SPSR, SPIF);
    }

    // 3. SK9822 reset frame
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);

    // 4. end frame
    n = size / 3 + 1;
    while (n--)
    {
        SPDR = 0x00;
        loop_until_bit_is_set(SPSR, SPIF);
    }

    CLRBITS(SPCR, BIT(SPE)); // disable SPI
    CS_LEAVE;
}

void sk9822SendRaw(const uint8_t *data, const uint16_t size)
{
    CS_ENTER;
    SETBITS(SPCR, BIT(SPE)); // enable SPI

    // 1. start frame
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);

    // 2. LED frames
    uint16_t n = size;
    while (n--)
    {
        SPDR = data != NULL ? *data++ : 0;
        loop_until_bit_is_set(SPSR, SPIF);
    }

    // 3. SK9822 reset frame
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);
    SPDR = 0x00;
    loop_until_bit_is_set(SPSR, SPIF);

    // 4. end frame
    n = size / 3 + 1;
    while (n--)
    {
        SPDR = 0x00;
        loop_until_bit_is_set(SPSR, SPIF);
    }

    CLRBITS(SPCR, BIT(SPE)); // disable SPI
    CS_LEAVE;


}

/* ************************************************************************** */

//@}
// eof

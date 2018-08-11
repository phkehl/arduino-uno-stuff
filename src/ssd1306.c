/*!
    \file
    \brief flipflip's Arduino Uno stuff: SSD1306 SSD1306 monochrome OLED display driver (see \ref SSD1306)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    Based on Adafruit's SSD1306 library for the Arduine (https://github.com/adafruit/Adafruit_SSD1306)

    - Copyright (c) Limor Fried/Ladyada, Adafruit Industries.

    \addtogroup SSD1306
    @{

*/

#include <string.h>        // libc: string operations
#include <stdarg.h>        // libc: variable argument lists

#include <util/twi.h>      // avr-libc: TWI bit mask definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "i2c.h"           // ff: i2c driver
#include "ssd1306.h"       // ff: SSD1306 driver

// check configuration
#if ( (FF_SSD1306_DISPLAY_SIZE != 6448) && (FF_SSD1306_DISPLAY_SIZE != 12864) && (FF_SSD1306_DISPLAY_SIZE != 12832) && (FF_SSD1306_DISPLAY_SIZE != 9616) )
#  error Illegal value for FF_SSD1306_DISPLAY_SIZE!
#endif
#if ( (FF_SSD1306_DISPLAY_ADDR != 0x3c) && (FF_SSD1306_DISPLAY_ADDR != 0x3d) )
#  error Illegal value for FF_SSD1306_DISPLAY_ADDR!
#endif
//#if ( (FF_SSD1306_DISPLAY_ORIENT < 0) || (FF_SSD1306_DISPLAY_ORIENT > 3) )
//#  error Illegal value for FF_SSD1306_DISPLAY_ORIENT!
//#endif

/* ************************************************************************** */

// display width and height
#if   (FF_SSD1306_DISPLAY_SIZE == 6448)
#  define SSD1306_WIDTH  64
#  define SSD1306_HEIGHT 48
#elif (FF_SSD1306_DISPLAY_SIZE == 12864)
#  define SSD1306_WIDTH  128
#  define SSD1306_HEIGHT 64
#elif (FF_SSD1306_DISPLAY_SIZE == 12832)
#  define SSD1306_WIDTH  128
#  define SSD1306_HEIGHT 32
#elif (FF_SSD1306_DISPLAY_SIZE == 9616)
#  define SSD1306_WIDTH  96
#  define SSD1306_HEIGHT 16
#else
#  error WTF?!
#endif

// fundamental commands
#define SSD1306_CMD_SET_CONTRAST      0x81 // set contrast control
#define SSD1306_CMD_DISP_ON_RESUME    0xa4 // entire display on, resume to RAM content display
#define SSD1306_CMD_DISP_ON_ALL       0xa5 // entire display on, ignore RAM content
#define SSD1306_CMD_DISP_NORMAL       0xa6 // set normal display
#define SSD1306_CMD_DISP_INVERT       0xa7 // set inverted display
#define SSD1306_CMD_DISP_OFF          0xae // display off (sleep)
#define SSD1306_CMD_DISP_ON           0xaf // display on in normal mode

// addressing setting
#define SSD1306_CMD_SET_MEM_MODE      0x20 // set memory addressing mode
#define SSD1306_CMD_SET_COL_ADDR      0x21 // set column address
#define SSD1306_CMD_SET_PAGE_ADDR     0x22 // set page address

// hardware configuration
#define SSD1306_CMD_START_LINE        0x40 // set display start line
#define SSD1306_CMD_SET_SEG_REMAP_0   0xa0 // set segment re-map, 0 is seg0
#define SSD1306_CMD_SET_SEG_REMAP_1   0xa1 // set segment re-map, 127 is seg0
#define SSD1306_CMD_SET_MULTIPLEX     0xa8 // set multiplex ration
#define SSD1306_CMD_SET_SCAN_DIR_NORM 0xc0 // set com output scan direction, normal
#define SSD1306_CMD_SET_SCAN_DIR_REM  0xc8 // set com output scan direction, remapped
#define SSD1306_CMD_SET_DISP_OFFS     0xd3 // set display offset
#define SSD1306_CMD_SET_COM_PINS      0xda // set com pins hardware configuration

// timing & driving scheme settings
#define SSD1306_CMD_SET_DISP_CLOCK    0xd5 // set display clock divide ratio / oscillator frequency
#define SSD1306_CMD_SET_PRE_CHARGE    0xd9 // set pre-charge period
#define SSD1306_CMD_SET_VCOM_LEVEL    0xdb // set vcomh delselect level
// 0xe3

// charge pump command table
#define SSD1306_CMD_CHARGE_PUMP       0x8d // charge pump setting

// scrolling commands
#define SSD1306_CMD_SCROLL_OFF        0x2e // deactivate scrolling

static void sSsd1306Command(uint8_t num, ...)
{
    i2cStart(FF_SSD1306_DISPLAY_ADDR, I2C_WRITE, 50);

    i2cWrite(0x00);

    va_list ap;
    va_start(ap, num);
    while (num > 0)
    {
        const uint8_t cmd = va_arg(ap, int);
        i2cWrite(cmd);
        num--;
    }
    va_end(ap);

    i2cStop();
}

/* ************************************************************************** */

inline uint16_t ssd1306Width(void)
{
    return SSD1306_WIDTH;
}

inline uint16_t ssd1306Height(void)
{
    return SSD1306_HEIGHT;
}

static uint8_t sSsd1306FrameBuffer[(SSD1306_WIDTH) * (SSD1306_HEIGHT) / 8];

void ssd1306Pixel(uint16_t x, uint16_t y, GFX_COLOUR_t colour)
{
    if ( (x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT) )
    {
        return;
    }

//#if   (FF_SSD1306_DISPLAY_ORIENT == 0)
//    // nothing
//#elif   (FF_SSD1306_DISPLAY_ORIENT == 1)
//    const uint16_t t = x;
//    x = y;
//    y = t;
//    x = SSD1306_WIDTH - 1 - x;
//#elif (FF_SSD1306_DISPLAY_ORIENT == 2)
//    x = SSD1306_WIDTH - 1 - x;
//    y = SSD1306_HEIGHT - 1 - y;
//#elif (FF_SSD1306_DISPLAY_ORIENT == 3)
//    const uint16_t t = x;
//    x = y;
//    y = SSD1306_HEIGHT - 1 - y;
//#else
//#  error WTF?!
//#endif

    switch (colour)
    {
        case GFX_WHITE:
            CLRBITS(sSsd1306FrameBuffer[x + (y / 8) * SSD1306_WIDTH], BIT(y & 7));
            break;
        case GFX_BLACK:
            SETBITS(sSsd1306FrameBuffer[x + (y / 8) * SSD1306_WIDTH], BIT(y & 7));
            break;
        case GFX_INVERT:
            TOGBITS(sSsd1306FrameBuffer[x + (y / 8) * SSD1306_WIDTH], BIT(y & 7));
            break;
        case GFX_TRANS:
            // do nothing, leave pixel as is
            break;
    }
}

void ssd1306Clear(void)
{
    memset(sSsd1306FrameBuffer, 0, sizeof(sSsd1306FrameBuffer));
}

void ssd1306Update(void)
{
    sSsd1306Command(3, SSD1306_CMD_SET_COL_ADDR,
#if   (FF_SSD1306_DISPLAY_SIZE == 6448)
        32, 32 + SSD1306_WIDTH - 1
#else
        0, SSD1306_WIDTH - 1
#endif
        );
    sSsd1306Command(3, SSD1306_CMD_SET_PAGE_ADDR, 0, (SSD1306_HEIGHT / 8));

#if 0
    // FIXME: The Adafruit library does this. Why?
    for (uint16_t ix = 0; ix < NUMOF(sSsd1306FrameBuffer); ix++)
    {
        if (!i2cStart(FF_SSD1306_DISPLAY_ADDR, I2C_WRITE, 50))
        {
            return;
        }
        i2cWrite(0x40);
        for (uint8_t x = 0; x < 16; x++)
        {
            i2cWrite(sSsd1306FrameBuffer[ix]);
            ix++;
        }
        ix--;
        i2cStop();
    }
#else
    if (!i2cStart(FF_SSD1306_DISPLAY_ADDR, I2C_WRITE, 50))
    {
        return;
    }
    i2cWrite(0x40);
    for (uint16_t ix = 0; ix < NUMOF(sSsd1306FrameBuffer); ix++)
    {
        i2cWrite(sSsd1306FrameBuffer[ix]);
    }
    i2cStop();
#endif
}

void ssd1306Invert(const bool invert)
{
    if (invert)
    {
        sSsd1306Command(1, SSD1306_CMD_DISP_INVERT);
    }
    else
    {
        sSsd1306Command(1, SSD1306_CMD_DISP_NORMAL);
    }
}

void ssd1306Dim(const bool dim)
{
    if (dim)
    {
        sSsd1306Command(2, SSD1306_CMD_SET_CONTRAST, 0);
    }
    else
    {
        sSsd1306Command(2, SSD1306_CMD_SET_CONTRAST,
#if   (FF_SSD1306_DISPLAY_SIZE == 6448)
            200
#elif (FF_SSD1306_DISPLAY_SIZE == 12864)
            200
#elif (FF_SSD1306_DISPLAY_SIZE == 12832)
            140
#elif (FF_SSD1306_DISPLAY_SIZE == 9616)
            175
#else
#  error WTF?!
#endif
            );
    }
}


/* ************************************************************************** */

void ssd1306Init(void)
{
    i2cInit();
    DEBUG("ssd1306: init (%"PRIu16"x%"PRIu16")", (uint16_t)SSD1306_WIDTH, (uint16_t)SSD1306_HEIGHT);

    // N.B.: some of ther ordering of the commands to setup the display is important. Not entirely sure what and why, though.

    // turn display off
    sSsd1306Command(1, SSD1306_CMD_DISP_OFF);

    // timing & driving scheme
    sSsd1306Command(2, SSD1306_CMD_SET_DISP_CLOCK, 0x80); // 0b1000... | 0b...000

    // addressing
    sSsd1306Command(2, SSD1306_CMD_SET_MEM_MODE, 0x00); // horizontal

    // hardware configuration
    sSsd1306Command(2, SSD1306_CMD_SET_MULTIPLEX, SSD1306_HEIGHT - 1); // FIXME: not sure why this works...
    sSsd1306Command(2, SSD1306_CMD_SET_DISP_OFFS, 0x00);
    sSsd1306Command(1, SSD1306_CMD_START_LINE | 0x00);
    sSsd1306Command(1, SSD1306_CMD_SET_SEG_REMAP_1);
    sSsd1306Command(1, SSD1306_CMD_SET_SCAN_DIR_REM);
#if   (FF_SSD1306_DISPLAY_SIZE == 6448)
    sSsd1306Command(2, SSD1306_CMD_SET_COM_PINS, 0x12); // 0b00010010 = alternative com pins, disable remap
#elif (FF_SSD1306_DISPLAY_SIZE == 12864)
    sSsd1306Command(2, SSD1306_CMD_SET_COM_PINS, 0x12); // 0b00010010 = alternative com pins, disable remap
#elif (FF_SSD1306_DISPLAY_SIZE == 12832)
    sSsd1306Command(2, SSD1306_CMD_SET_COM_PINS, 0x02); // 0b00000010 = sequential com pins, disable remap
#elif (FF_SSD1306_DISPLAY_SIZE == 9616)
    sSsd1306Command(2, SSD1306_CMD_SET_COM_PINS, 0x02); // 0b00000010 = sequential com pins, disable remap
#else
#  error WTF?!
#endif

    // other
    ssd1306Dim(false);

    // charge pump
    sSsd1306Command(2, SSD1306_CMD_CHARGE_PUMP, 0x14); // charge pump on (0b..010100 = 0x14)
    sSsd1306Command(2, SSD1306_CMD_SET_PRE_CHARGE, 0xf1); // 0b1111.... | 0b....0001
    sSsd1306Command(2, SSD1306_CMD_SET_VCOM_LEVEL, 0x40); // 0b.100.... // FIXME: illegal value?

    // almost done..
    sSsd1306Command(1, SSD1306_CMD_DISP_ON_RESUME);
    sSsd1306Command(1, SSD1306_CMD_DISP_NORMAL);
    sSsd1306Command(1, SSD1306_CMD_SCROLL_OFF);
    sSsd1306Command(1, SSD1306_CMD_DISP_ON);

    // blank display
    ssd1306Clear();
    ssd1306Update();
}

/* ************************************************************************** */

//@}
// eof

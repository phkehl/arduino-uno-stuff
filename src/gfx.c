/*!
    \file
    \brief flipflip's Arduino Uno stuff: graphics primitives (see \ref GFX)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    Some functions based on Adafruit's SSD1306 library for the Arduine (https://github.com/adafruit/Adafruit_SSD1306)

    - Copyright (c) Limor Fried/Ladyada, Adafruit Industries.

    \addtogroup GFX
    @{

    \todo implement display orientation
    \todo add gfxPrint_P()

*/

#include <string.h>        // libc: string operations

#include <ada-gfx-font.h>  // 3rd-party: Adafruit GFX font

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "debug.h"         // ff: debugging
#include "gfx.h"           // ff: graphics primitives


/* ************************************************************************** */

static GFX_DRV_t sGfxDrv;

void gfxInit(const GFX_DRV_t *pDrv)
{
    sGfxDrv = *pDrv;
    DEBUG("gfx: init (%"PRIu16"x%"PRIu16")", gfxWidth(), gfxHeight());
}

inline void gfxClear(void)
{
    sGfxDrv.clearFunc();
}

inline int16_t gfxWidth(void)
{
    return sGfxDrv.rawWidth;
}

inline int16_t gfxHeight(void)
{
    return sGfxDrv.rawHeight;
}

inline void gfxUpdate(void)
{
    sGfxDrv.updateFunc();
}

inline void gfxPixel(int16_t x, int16_t y, GFX_COLOUR_t colour)
{
    sGfxDrv.pixelFunc((uint16_t)x, (uint16_t)y, colour);
}

/* ************************************************************************** */

void gfxLineH(int16_t x, int16_t y, int16_t w, GFX_COLOUR_t colour)
{
    if ( (x < 0) || (y < 0) || (x >= gfxWidth()) || (y >= gfxHeight()) || (w < 0) )
    {
        return;
    }
    int16_t x1 = x + w;
    if (x1 >= gfxWidth())
    {
        x1 = gfxWidth() - 1;
    }
    for (; x <= x1; x++)
    {
        gfxPixel(x, y, colour);
    }
}

void gfxLineV(int16_t x, int16_t y, int16_t h, GFX_COLOUR_t colour)
{
    if ( (x < 0) || (y < 0) || (x >= gfxWidth()) || (y >= gfxHeight()) || (h < 0) )
    {
        return;
    }
    int16_t y1 = y + h;
    if (y1 >= gfxHeight())
    {
        y1 = gfxHeight() - 1;
    }
    for (; y <= y1; y++)
    {
        gfxPixel(x, y, colour);
    }
}

void gfxRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour)
{
    if ( (x0 < 0) || (y0 < 0) || (x1 <= x0) || (y1 <= y0) )
    {
        return;
    }
    gfxLineV(x0, y0, y1 - y0, colour);
    gfxLineV(x1, y0, y1 - y0, colour);
    gfxLineH(x0, y0, x1 - x0, colour);
    gfxLineH(x0, y1, x1 - x0, colour);
}

void gfxFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour)
{
    if ( (x0 < 0) || (y0 < 0) || (x1 <= x0) || (y1 <= y0) )
    {
        return;
    }
    for (int16_t x = x0; x <= x1; x++)
    {
        for (int16_t y = y0; y <= y1; y++)
        {
            gfxPixel(x, y, colour);
        }
    }
}

#define _SWAP(a, b) { typeof(a) t = a; a = b; b = t; }

// based on https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
// modified code from Adafruit_GFX::writeLine(), (c) Adafruit (see header)
void gfxLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour)
{
    if ( (x0 < 0) || (x0 >= gfxWidth())  || (x1 < 0) || (x1 >= gfxWidth()) ||
         (y0 < 0) || (y0 >= gfxHeight()) || (y1 < 0) || (y1 >= gfxHeight()) )
    {
        return;
    }
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    if ( (dx < 1) || (dy < 1) )
    {
        return;
    }
    if (dx == 1)
    {
        gfxLineV(x0, y0, dy, colour);
        return;
    }
    if (dy == 1)
    {
        gfxLineH(x0, y0, y1, colour);
        return;
    }

    dy = ABS(dy);

    bool steep = dy > ABS(dx);
    if (steep)
    {
        _SWAP(x0, y0);
        _SWAP(x1, y1);
    }

    if (x0 > x1)
    {
        _SWAP(x0, x1);
        _SWAP(y0, y1);
    }

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }
    for (; x0 <= x1; x0++)
    {
        if (steep)
        {
            gfxPixel(y0, x0, colour);
        }
        else
        {
            gfxPixel(x0, y0, colour);
        }
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}

static void sGfxPrint5x7(uint8_t size, int16_t x, int16_t y, GFX_COLOUR_t fg, GFX_COLOUR_t bg, const char *str)
{
    // skip early, gfxPixel() will take care of the precise bounds
    if ( (x < 0) || (y < 0) || (x >= gfxWidth()) || (y >= gfxHeight()) )
    {
        return;
    }
    if ( (size < 1) || (size > 3) )
    {
        return;
    }
    const int iLen = strlen(str);
    if ( (iLen > 255) || (iLen < 1) )
    {
        return;
    }
    const uint8_t len = (uint8_t)iLen;
    for (uint8_t strIx = 0; strIx < len; strIx++)
    {
        // draw character...
        uint8_t *bitmap = adaGfxFontChar(str[strIx]);
        for (uint8_t bitmapIx = 0; bitmapIx < 5; bitmapIx++)
        {
            // ...row by row
            uint8_t row = bitmap[bitmapIx];
            uint8_t moy = 8 * size;
            for (uint8_t oy = 0; oy < moy; oy += size)
            {
                const bool isSet = (row & 0x01) == 0x01;
                if (size == 1)
                {
                    gfxPixel(x, y + oy, isSet ? fg : bg);
                }
                else
                {
                    gfxFill(x, y + oy, x + size - 1, y + oy + size - 1, isSet ? fg : bg);
                }
                row >>= 1;
            }
            x += size;
        }
        // draw padding
        if (strIx < (len - 1))
        {
            gfxLineV(x, y, 8, bg);
            x += size;
        }
    }
}

void gfxPrint(GFX_FONT_t font, int16_t x, int16_t y, GFX_COLOUR_t fg, GFX_COLOUR_t bg, const char *str)
{
    switch (font)
    {
        case GFX_FONT_5X7_1:
            sGfxPrint5x7(1, x, y, fg, bg, str);
            break;
        case GFX_FONT_5X7_2:
            sGfxPrint5x7(2, x, y, fg, bg, str);
            break;
        case GFX_FONT_5X7_3:
            sGfxPrint5x7(3, x, y, fg, bg, str);
            break;
    }
}


/* ************************************************************************** */

//@}
// eof

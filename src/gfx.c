/*!
    \file
    \brief flipflip's Arduino Uno stuff: graphics primitives (see \ref GFX)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup GFX
    @{

    \todo implement display orientation
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

inline uint16_t gfxWidth(void)
{
    return sGfxDrv.rawWidth;
}

inline uint16_t gfxHeight(void)
{
    return sGfxDrv.rawHeight;
}

inline void gfxUpdate(void)
{
    sGfxDrv.updateFunc();
}

inline void gfxSetPixel(uint16_t x, uint16_t y, GFX_COLOUR_t colour)
{
    sGfxDrv.setpixelFunc(x, y, colour);
}

/* ************************************************************************** */

void gfxLineH(uint16_t x, uint16_t y, uint16_t w, GFX_COLOUR_t colour)
{
    if ( (x >= gfxWidth()) || (y >= gfxHeight()) )
    {
        return;
    }
    uint16_t x1 = x + w;
    if (x1 >= gfxWidth())
    {
        x1 = gfxWidth() - 1;
    }
    for (; x <= x1; x++)
    {
        gfxSetPixel(x, y, colour);
    }
}

void gfxLineV(uint16_t x, uint16_t y, uint16_t h, GFX_COLOUR_t colour)
{
    if ( (x >= gfxWidth()) || (y >= gfxHeight()) )
    {
        return;
    }
    uint16_t y1 = y + h;
    if (y1 >= gfxHeight())
    {
        y1 = gfxHeight() - 1;
    }
    for (; y <= y1; y++)
    {
        gfxSetPixel(x, y, colour);
    }
}

void gfxLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour)
{
    UNUSED(x0); UNUSED(y0); UNUSED(x1); UNUSED(y1); UNUSED(colour);
    // TODO: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    /*
plotLineLow(x0,y0, x1,y1)
  dx = x1 - x0
  dy = y1 - y0
  yi = 1
  if dy < 0
    yi = -1
    dy = -dy
  end if
  D = 2*dy - dx
  y = y0

  for x from x0 to x1
    plot(x,y)
    if D > 0
       y = y + yi
       D = D - 2*dx
    end if
    D = D + 2*dy

plotLineHigh(x0,y0, x1,y1)
  dx = x1 - x0
  dy = y1 - y0
  xi = 1
  if dx < 0
    xi = -1
    dx = -dx
  end if
  D = 2*dx - dy
  x = x0

  for y from y0 to y1
    plot(x,y)
    if D > 0
       x = x + xi
       D = D - 2*dy
    end if
    D = D + 2*dx

plotLine(x0,y0, x1,y1)
  if abs(y1 - y0) < abs(x1 - x0)
    if x0 > x1
      plotLineLow(x1, y1, x0, y0)
    else
      plotLineLow(x0, y0, x1, y1)
    end if
  else
    if y0 > y1
      plotLineHigh(x1, y1, x0, y0)
    else
      plotLineHigh(x0, y0, x1, y1)
    end if
  end if

     */
}

void gfxRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour)
{
    if ( (x1 <= x0) || (y1 <= y0) )
    {
        return;
    }
    gfxLineV(x0, y0, y1 - y0, colour);
    gfxLineV(x1, y0, y1 - y0, colour);
    gfxLineH(x0, y0, x1 - x0, colour);
    gfxLineH(x0, y1, x1 - x0, colour);
}

void gfxFill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour)
{
    if ( (x1 <= x0) || (y1 <= y0) )
    {
        return;
    }
    for (uint16_t x = x0; x <= x1; x++)
    {
        for (uint16_t y = y0; y <= y1; y++)
        {
            gfxSetPixel(x, y, colour);
        }
    }
}

void gfxPrint(uint16_t x, uint16_t y, uint8_t size, GFX_COLOUR_t colour, const char *str)
{
    // skip early, gfxSetPixel() will take care of the precise bounds
    if ( (x >= gfxWidth()) || (y >= gfxHeight()) )
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
        uint8_t *bitmap = adaGfxFontChar(str[strIx]);
        for (uint8_t bitmapIx = 0; bitmapIx < 5; bitmapIx++)
        {
            uint8_t row = bitmap[bitmapIx];
            for (uint8_t oy = 0; oy < 8; oy++)
            {
                const bool isSet = (row & 0x01) == 0x01;
                switch (colour)
                {
                    case GFX_BLACK:
                        gfxSetPixel(x, y + oy, isSet ? GFX_BLACK : GFX_WHITE);
                        break;
                    case GFX_WHITE:
                        gfxSetPixel(x, y + oy, isSet ? GFX_WHITE : GFX_BLACK);
                        break;
                    case GFX_INVERT:
                        gfxSetPixel(x, y + oy, GFX_INVERT);
                        break;
                    case GFX_TRANS:
                        if (isSet)
                        {
                            gfxSetPixel(x, y + oy, GFX_INVERT);
                        }
                        break;
                }
                row >>= 1;
            }
            x++;
        }
        x += size;
    }
}


/* ************************************************************************** */

//@}
// eof

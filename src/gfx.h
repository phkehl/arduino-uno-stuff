/*!
    \file
    \brief flipflip's Arduino Uno stuff: graphics primitives (see \ref GFX)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    Some functions based on Adafruit's SSD1306 library for the Arduine (https://github.com/adafruit/Adafruit_SSD1306)

    - Copyright (c) Limor Fried/Ladyada, Adafruit Industries.

    \defgroup GFX graphics primitives
    \ingroup FF

    This implements graphics primitives (dots, lines, boxes, etc.), e.g. for \ref SSD1306.

    See \ref EXAMPLES_EX15 for a demonstration.

    @{
*/

#ifndef __GFX_H__
#define __GFX_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

typedef enum GFX_COLOUR_e
{
    GFX_WHITE, GFX_BLACK, GFX_INVERT, GFX_TRANS,

} GFX_COLOUR_t;


typedef void (* GFX_DRV_CLEAR_FUNC_t)(void);
typedef void (* GFX_DRV_UPDATE_FUNC_t)(void);
typedef void (* GFX_DRV_PIXEL_FUNC_t)(uint16_t, uint16_t, GFX_COLOUR_t);

typedef struct GFX_DRV_s
{
    GFX_DRV_CLEAR_FUNC_t  clearFunc;
    GFX_DRV_UPDATE_FUNC_t updateFunc;
    GFX_DRV_PIXEL_FUNC_t  pixelFunc;
    int16_t               rawWidth;
    int16_t               rawHeight;
} GFX_DRV_t;

#define GFX_DRV(_clearFunc, _updateFunc, _pixelFunc, _rawWidth, _rawHeight) \
    { .clearFunc = (_clearFunc), .updateFunc = (_updateFunc), .pixelFunc = (_pixelFunc), .rawWidth = (_rawWidth), .rawHeight = (_rawHeight) }

//! initialise things
void gfxInit(const GFX_DRV_t *pDrv);

void gfxClear(void);
void gfxUpdate(void);

int16_t gfxWidth(void);
int16_t gfxHeight(void);

void gfxPixel(int16_t x, int16_t y, GFX_COLOUR_t colour);

void gfxLineH(int16_t x, int16_t y, int16_t w, GFX_COLOUR_t colour);
void gfxLineV(int16_t x, int16_t y, int16_t h, GFX_COLOUR_t colour);
void gfxLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);
void gfxRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);
void gfxFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);

void gfxPrint(int16_t x, int16_t y, uint8_t size, GFX_COLOUR_t colour, const char *str);

/* *************************************************************************** */

#endif // __GFX_H__

//@}
// eof

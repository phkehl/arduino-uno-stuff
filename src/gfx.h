/*!
    \file
    \brief flipflip's Arduino Uno stuff: graphics primitives (see \ref GFX)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

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
typedef void (* GFX_DRV_SETPIXEL_FUNC_t)(uint16_t, uint16_t, GFX_COLOUR_t);

typedef struct GFX_DRV_s
{
    GFX_DRV_CLEAR_FUNC_t    clearFunc;
    GFX_DRV_UPDATE_FUNC_t   updateFunc;
    GFX_DRV_SETPIXEL_FUNC_t setpixelFunc;
    uint16_t                rawWidth;
    uint16_t                rawHeight;
} GFX_DRV_t;

#define GFX_DRV(_clearFunc, _updateFunc, _setpixelFunc, _rawWidth, _rawHeight) \
    { .clearFunc = (_clearFunc), .updateFunc = (_updateFunc), .setpixelFunc = (_setpixelFunc), .rawWidth = (_rawWidth), .rawHeight = (_rawHeight) }

//! initialise things
void gfxInit(const GFX_DRV_t *pDrv);

void gfxClear(void);
void gfxUpdate(void);

uint16_t gfxWidth(void);
uint16_t gfxHeight(void);

void gfxSetPixel(uint16_t x, uint16_t y, GFX_COLOUR_t colour);

void gfxLineH(uint16_t x, uint16_t y, uint16_t w, GFX_COLOUR_t colour);
void gfxLineV(uint16_t x, uint16_t y, uint16_t h, GFX_COLOUR_t colour);
void gfxLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour);
void gfxRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour);
void gfxFill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, GFX_COLOUR_t colour);

void gfxPrint(uint16_t x, uint16_t y, uint8_t size, GFX_COLOUR_t colour, const char *str);

/* *************************************************************************** */

#endif // __GFX_H__

//@}
// eof

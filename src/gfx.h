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

//! colours
typedef enum GFX_COLOUR_e
{
    GFX_WHITE,  //!< white (foreground colour, pixel set for monochrome displays)
    GFX_BLACK,  //!< black (background colour, pixel clear for monochrome displays)
    GFX_INVERT, //!< invert existing colour (#GFX_WHITE <--> #GFX_BLACK)
    GFX_TRANS,  //!< transparent (leave pixel as it is)
} GFX_COLOUR_t;


//! function that clears the physical display
typedef void (* GFX_DRV_CLEAR_FUNC_t)(void);

//! function that updates the physical display (writes the frame buffer to the display)
typedef void (* GFX_DRV_UPDATE_FUNC_t)(void);

//! function that sets a pixel on the physical display frame buffer
typedef void (* GFX_DRV_PIXEL_FUNC_t)(uint16_t, uint16_t, GFX_COLOUR_t);

//! gfx driver (information on what controls the physical display)
typedef struct GFX_DRV_s
{
    GFX_DRV_CLEAR_FUNC_t  clearFunc;  //!< function that clears the physical display
    GFX_DRV_UPDATE_FUNC_t updateFunc; //!< function that updates the physical display (writes the frame buffer to the display)
    GFX_DRV_PIXEL_FUNC_t  pixelFunc;  //!< function that sets a pixel on the physical display frame buffer
    int16_t               rawWidth;   //!< width in [px] of the physical display
    int16_t               rawHeight;  //!< height in [px] of the physical display
} GFX_DRV_t;

//! creates an initialiser for a #GFX_DRV_t structure
#define GFX_DRV(_clearFunc, _updateFunc, _pixelFunc, _rawWidth, _rawHeight) \
    { .clearFunc = (_clearFunc), .updateFunc = (_updateFunc), .pixelFunc = (_pixelFunc), .rawWidth = (_rawWidth), .rawHeight = (_rawHeight) }

//! initialise things
/*!
    \param[in] pDrv  pointer to the driver structure

    \note This must be called before any of the other functions may be called.

    \note None of the gfx functions check coordinate bounds. Make sure that the
          \ref GFX_DRV_PIXEL_FUNC_t does check bounds if necessary.
*/
void gfxInit(const GFX_DRV_t *pDrv);

//! clear frame buffer
void gfxClear(void);

//! write frame buffer to the display
void gfxUpdate(void);

//! width of the canvas
/*!
    \returns the width in [px] of the canvas
*/
int16_t gfxWidth(void);

//! height of the canvas
/*!
    \returns the height in [px] of the canvas
*/
int16_t gfxHeight(void);

//! write a pixel
/*!
    Coordinates are zero based. The origin (x/y = 0/0) is the top left corner of the display. X is
    horizontal (right), y vertical (down).

    \param[in] x       x coordinate of the pixel
    \param[in] y       y coordinate of the pixel
    \param[in] colour  colour to change the pixel to
*/
void gfxPixel(int16_t x, int16_t y, GFX_COLOUR_t colour);

//! draw a horizontal line
/*!
    \param[in] x       start of line x coordinate
    \param[in] y       start of line y coordinate
    \param[in] w       width of line [px], must be > 0
    \param[in] colour  colour to draw the line with
*/
void gfxLineH(int16_t x, int16_t y, int16_t w, GFX_COLOUR_t colour);

//! draw a vertical line
/*!
    \param[in] x       start of line x coordinate
    \param[in] y       start of line y coordinate
    \param[in] h       height of line [px], must be > 0
    \param[in] colour  colour to draw the line with
*/
void gfxLineV(int16_t x, int16_t y, int16_t h, GFX_COLOUR_t colour);

//! draw a line (from to anywhere)
/*!
    \param[in] x0      start of line x coordinate
    \param[in] y0      start of line y coordinate
    \param[in] x1      end of line x coordinate
    \param[in] y1      end of line y coordinate
    \param[in] colour  colour to draw the line with
*/
void gfxLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);

//! draw a rectangle (outline)
/*!
    \param[in] x0      x coordinate of top left corner
    \param[in] y0      y coordinate of top left corner
    \param[in] x1      x coordinate of bottom right corner, must be >= \c x0
    \param[in] y1      y coordinate of bottom right corner, must be >= \c y0
    \param[in] colour  colour to draw the lines with
*/
void gfxRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);

//! draw a filled rectangle
/*!
    \param[in] x0      x coordinate of top left corner
    \param[in] y0      y coordinate of top left corner
    \param[in] x1      x coordinate of bottom right corner, must be >= \c x0
    \param[in] y1      y coordinate of bottom right corner, must be >= \c y0
    \param[in] colour  colour to fille the rectangle with
*/
void gfxFill(int16_t x0, int16_t y0, int16_t x1, int16_t y1, GFX_COLOUR_t colour);

//! fonts
typedef enum GFX_FONT_e
{
    GFX_FONT_5X7_1, //!< 5x7 px fixed-width font, unscaled
    GFX_FONT_5X7_2, //!< 5x7 px fixed-width font, scaled by factor 2
    GFX_FONT_5X7_3, //!< 5x7 px fixed-width font, scaled by factor 3
} GFX_FONT_t;

//! print a string
/*!
    Characters are 5 [px] wide and 7 [px] tall. Individual characters are separated by 1 [px].
    The \c size will scales these accordingly.

    \param[in] font    the font to use
    \param[in] x       x coordinate of top left corner of the first letter
    \param[in] y       y coordinate of top left corner of the first letter
    \param[in] fg      foreground colour (letter outlines)
    \param[in] bg      background colour
    \param[in] str     the string
*/
void gfxPrint(GFX_FONT_t font, int16_t x, int16_t y, GFX_COLOUR_t fg, GFX_COLOUR_t bg, const char *str);

//! print a string from progmem
/*!
    Characters are 5 [px] wide and 7 [px] tall. Individual characters are separated by 1 [px].
    The \c size will scales these accordingly.

    \param[in] font    the font to use
    \param[in] x       x coordinate of top left corner of the first letter
    \param[in] y       y coordinate of top left corner of the first letter
    \param[in] fg      foreground colour (letter outlines)
    \param[in] bg      background colour
    \param[in] str     the string
*/
void gfxPrint_P(GFX_FONT_t font, int16_t x, int16_t y, GFX_COLOUR_t fg, GFX_COLOUR_t bg, const char *str);

/* *************************************************************************** */

#endif // __GFX_H__

//@}
// eof

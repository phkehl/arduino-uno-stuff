/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED effects (see \ref LEDFX)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    - Portions copyright by others (see ledfx.h)

    \defgroup LEDFX LED Effects
    \ingroup FF

    This implements effects for LED matrices and strings.

    Configuration:
    - #FF_LEDFX_NUM_LED
    - #FF_LEDFX_ORDER
    - #FF_LEDFX_NUM_X
    - #FF_LEDFX_NUM_Y
    - #FF_LEDFX_XY_ARR
    - (#FF_HSV2RGB_METHOD)

    Initialise the hwMathGetRandom() random number generator before using these functions:
\code{.c}
hwMathSeedRandom(hwGetRandomSeed());
\endcode

Supported LED matrix arrangements for LED electrically connected as 0-1-2-3-4-5-6-7-8 (x/y 0/0 is at
the bottom left, values for #FF_LEDFX_XY_ARR):

\verbatim
   y          (1)     (2)     (3)     (4)
   ^         6 7 8   6 7 8   8 7 6   8 7 6
   |         3 4 5   5 4 3   3 4 5   5 4 3
   |         0 1 2   0 1 2   2 1 0   2 1 0
   +---->x
(0,0)
\endverbatim

    @{
*/

#ifndef __LEDFX_H__
#define __LEDFX_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */
/*!
    \name Frame Buffer Functions
    @{
*/

//! clear frame buffer
/*!
    \param[in] ix0  start index
    \param[in] ix1  end index

    Setting both \c from and \c to to 0 will clear all.
*/
void ledfxClear(const uint16_t ix0, const uint16_t ix1);

//! get number of LEDs in frame buffer
uint16_t ledfxNumLeds(void);

//! get pointer to frame buffer
const uint8_t *ledfxGetFrameBuffer(void);

//! get size of frame buffer
uint16_t ledfxGetFrameBufferSize(void);

//! set global maximum brightness
/*!
    \param[in] brightness  maximum brightness (1..254), (0 or 255 disables the scaling)

    \todo something's not quite working here..
*/
void ledfxSetBrightness(const uint8_t brightness);

//! set pixel given red, green and blue (RGB)
/*!
    \param[in] ix     LED index (0..(#FF_LEDFX_NUM_LED-1))
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)
*/
void ledfxSetIxRGB(const uint16_t ix, const uint8_t red, const uint8_t green, const uint8_t blue);

//! set pixel given hue, saturation and value (HSV)
/*!
    \param[in] ix   LED index (0..(#FF_LEDFX_NUM_LED-1))
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)
*/
void ledfxSetIxHSV(const uint16_t ix, const uint8_t hue, const uint8_t sat, const uint8_t val);

//! set matrix pixel given hue, saturation and value (HSV)
/*!
    \param[in] x    LED column index (0..(#FF_LEDFX_NUM_X-1))
    \param[in] y    LED row index (0..(#FF_LEDFX_NUM_Y-1))
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)
*/
void ledfxSetMatrixHSV(const uint16_t x, const uint16_t y, const uint8_t hue, const uint8_t sat, const uint8_t val);

//! set matrix pixel given red, green and blue (RGB)
/*!
    \param[in] x      LED column index (0..(#FF_LEDFX_NUM_X-1))
    \param[in] y      LED row index (0..(#FF_LEDFX_NUM_Y-1))
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)
*/
void ledfxSetMatrixRGB(const uint16_t x, const uint16_t y, const uint8_t red, const uint8_t green, const uint8_t blue);

//! fill range of pixels given red, green and blue (RGB)
/*!
    \param[in] ix0    start index
    \param[in] ix1    end index
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)

    Setting both \c ix0 and \c ix1 to to 0 will fill all.
*/
void ledfxFillRGB(const uint16_t ix0, const uint16_t ix1, const uint8_t red, const uint8_t green, const uint8_t blue);

//! fill range of pixels given hue, saturation and value (HSV)
/*!
    \param[in] ix0  start index
    \param[in] ix1  end index
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)

    Setting both \c ix0 and \c ix1 to to 0 will fill all.
*/
void ledfxFillHSV(const uint16_t ix0, const uint16_t ix1, const uint8_t hue, const uint8_t sat, const uint8_t val);

//! limit maximum current drawn by the LEDs
/*!
    This limits the maximum current drawn by calculating the current per LED based on the \c
    maPerLed value, which is the current one LED draws if all three channels are at maximum output
    (i.e. RGB=(255, 255, 255)). It assumes a linear behaviour and equality of all channels.
    In reality it's not linear and the different channels have different current drains.

    See ledfx.c for some measurements.

    \param[in]  maPerLed  milliamps per LED when fully lit
    \param[in]  maMax     maximum milliamps for the whole matrix/strip
    \param[out] pMaUsed   estimate of the actual current drain (or #NULL)

    \returns the number of LEDs limited
*/
uint16_t ledfxLimitCurrent(const uint16_t maPerLed, const uint16_t maMax, uint16_t *pMaUsed);

//@}


/* *************************************************************************** */
/*!
    \name Noise Effects

    @{
*/

//! random colour noise
/*!
    \param[in] init  set to \c true on first call to initialise things
    \param[in] ix0   start index
    \param[in] ix1   end index
    \param[in] num   number of LEDs to change colours in one call

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxNoiseRandom(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num);

//! random distinct colour noise
/*!
    \param[in] init  set to \c true on first call to initialise things
    \param[in] ix0   start index
    \param[in] ix1   end index
    \param[in] num   number of LEDs to change colours in one call

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxNoiseRandomDistinct(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num);

//! moving hue noise
/*!
    \param[in]      init  set to \c true on first call to initialise things
    \param[in]      ix0   start index
    \param[in]      ix1   end index
    \param[in]      num   number of LEDs to change colours in one call
    \param[in,out]  r0    effect state storage
    \param[in,out]  r1    effect state storage

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxNoiseMovingHue(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num, uint8_t *r0, uint8_t *r1);

//! (pretty cheap) stoboscope effect
/*!
    \param[in]      init  set to \c true on first call to initialise things
    \param[in]      ix0   start index
    \param[in]      ix1   end index
    \param[in,out]  r0    effect state storage

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxStrobo(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0);

//@}


/* *************************************************************************** */
/*!
    \name Colour Flow Effects

    @{
*/

//! concentric moving hue flow
/*!
    \todo make this centred for even numbers of rows/columns

    \param[in]      init  set to \c true on first call to initialise things
    \param[in]      step  how many hue steps to move per iteration
    \param[in,out]  r0    effect state storage
*/
void ledfxConcentricHueFlow(const bool init, const int8_t step, uint8_t *r0);

//! plasma effect
/*!
    \param[in]       init  set to \c true on first call to initialise things
    \param[in,out]   r0    effect state storage

    The formulas used in this function are based on code floating the internet in various code
    (google "colorduino", "shiftbrite", et al.). While the original source is never properly
    referenced it all seems to be attributed to: copyright (c) 2011 Sam C. Lin, 2009 Ben Combee,
    2009 Ken Corey, 2008 Windell H. Oskay \todo find original reference for this
*/
void ledfxPlasma(const bool init, float *r0);

//! rainbow effect
/*!
    \param[in]      init  set to \c true on first call to initialise things
    \param[in]      ix0   start index
    \param[in]      ix1   end index
    \param[in,out]  r0    effect state storage

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxRainbow(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0);

//! (pretty cheap) hue sweep
/*!
    \param[in]      init  set to \c true on first call to initialise things
    \param[in]      ix0   start index
    \param[in]      ix1   end index
    \param[in,out]  r0    effect state storage

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxHueSweep(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0);

//! waves effect
/*!
    \param[in]      init  set to \c true on first call to initialise things
    \param[in,out]  r0    effect state storage
    \param[in,out]  r1    effect state storage
    \param[in,out]  r2    effect state storage
*/
void ledfxWaves(const bool init, uint8_t *r0, float *r1, float *r2);

//@}


/* *************************************************************************** */
/*!
    \name Matrix Only Effects

    @{
*/

//! rotor effect
/*!
    \param[in]     init  set to \c true on first call to initialise things
    \param[in,out] r0    effect state storage
    \param[in,out] r1    effect state storage
*/
void ledfxRotor(const bool init, float *r0, float *r1);

//! diagonal hue change effect
/*!
    \param[in]     init  set to \c true on first call to initialise things
    \param[in,out] r0    effect state storage
*/
void ledfxDiagonal(const bool init, uint8_t *r0);

//! rain effect state
typedef struct LEDFX_RAIN_s
{
    int8_t pos[FF_LEDFX_NUM_X];
    uint8_t hue[FF_LEDFX_NUM_X];
    uint8_t len[FF_LEDFX_NUM_X];
    int8_t f;
    uint8_t n;
} LEDFX_RAIN_t;

//! rain effect
/*!
    \param[in]     init    set to \c true on first call to initialise things
    \param[in,out] pState  raindrop state memory (effect state storage)
*/
void ledfxRain(const bool init, LEDFX_RAIN_t *pState);

//! a star
typedef struct LEDFX_STAR_s
{
    uint16_t ix;
    uint8_t hue;
    uint8_t val;
    uint8_t valMax;
    int8_t speed;
} LEDFX_STAR_t;

//! stars effect
/*!
    \param[in]     init    set to \c true on first call to initialise things
    \param[in,out] pStars  stars state memory
    \param[in]     nStars  number of stars (number of \c *pStars)
*/
void ledfxStars(const bool init, LEDFX_STAR_t *pStars, const uint16_t nStars);

//! render digit
/*!
    The digits are 4x7 pixels

    \param[in] digit  the digit to render (0..9)
    \param[in] x0     x offset
    \param[in] y0     y offset
    \param[in] hue    hue value (0..255)
    \param[in] sat    saturation value (0..255)
    \param[in] val    (brightness) value (0..255)
*/
void ledfxDigit(const uint8_t digit, const uint16_t x0, const uint16_t y0,
    const uint8_t hue, const uint8_t sat, const uint8_t val);


//! random fill effect state
typedef struct LEDFX_RANDFILL_s
{
    int16_t  x;
    int16_t  y;
    uint8_t  hue;
    uint8_t  mode;
} LEDFX_RANDFILL_t;

//! random fill effect
/*!
    \param[in]     init    set to \c true on first call to initialise things
    \param[in,out] pState  raindrop state memory (effect state storage)
*/
void ledfxRandFill(const bool init, LEDFX_RANDFILL_t *pState);


//@}


/* *************************************************************************** */

#endif // __LEDFX_H__

//@}
// eof

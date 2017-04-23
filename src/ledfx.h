/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED effects (see \ref LEDFX)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    - Portions copyright by others (see ledfx.h)

    \defgroup LEDFX LED Effects
    \ingroup FF

    This implements effects for LED matrices and strings. Adjust #FF_LEDFX_NUM_LED, #FF_LEDFX_ORDER,
    and in case of an LED matrix also #FF_LEDFX_NUM_X, #FF_LEDFX_NUM_Y and #FF_LEDFX_XY_ARR, for
    your setup. The "HSV" routines are affected by #FF_HSV2RGB_METHOD.

    Initialise the hwMathGetRandom() random number generator before using these functions:
\code{.c}
hwMathSeedRandom(hwGetRandomSeed());
\endcode

Supported LED matrix arrangements (x/y 0/0 is at the bottom left, values for #FF_LEDFX_XY_ARR):

\verbatim
 (1)     (2)
6 7 8   6 7 8
3 4 5   5 4 3
0 1 2   0 1 2
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
void ledfxClear(const U2 ix0, const U2 ix1);

//! get number of LEDs in frame buffer
U2 ledfxNumLeds(void);

//! get pointer to frame buffer
const U1 *ledfxGetFrameBuffer(void);

//! get size of frame buffer
U2 ledfxGetFrameBufferSize(void);

//! set global maximum brightness
/*!
    \param[in] brightness  maximum brightness (1..254), (0 or 255 disables the scaling)

    \todo something's not quite working here..
*/
void ledfxSetBrightness(const U1 brightness);

//! set pixel given red, green and blue (RGB)
/*!
    \param[in] ix     LED index (0..(#FF_LEDFX_NUM_LED-1))
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)
*/
void ledfxSetRGB(const U2 ix, const U1 red, const U1 green, const U1 blue);

//! set pixel given hue, saturation and value (HSV)
/*!
    \param[in] ix   LED index (0..(#FF_LEDFX_NUM_LED-1))
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)
*/
void ledfxSetHSV(const U2 ix, const U1 hue, const U1 sat, const U1 val);

//! set matrix pixel given hue, saturation and value (HSV)
/*!
    \param[in] x    LED column index (0..(#FF_LEDFX_NUM_X-1))
    \param[in] y    LED row index (0..(#FF_LEDFX_NUM_Y-1))
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)
*/
void ledfxSetMatrixHSV(const U2 x, const U2 y, const U1 hue, const U1 sat, const U1 val);

//! set matrix pixel given red, green and blue (RGB)
/*!
    \param[in] x      LED column index (0..(#FF_LEDFX_NUM_X-1))
    \param[in] y      LED row index (0..(#FF_LEDFX_NUM_Y-1))
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)
*/
void ledfxSetMatrixRGB(const U2 x, const U2 y, const U1 red, const U1 green, const U1 blue);

//! fill range of pixels given red, green and blue (RGB)
/*!
    \param[in] ix0    start index
    \param[in] ix1    end index
    \param[in] red    red value (0..255)
    \param[in] green  green value (0..255)
    \param[in] blue   blue value (0..255)

    Setting both \c ix0 and \c ix1 to to 0 will fill all.
*/
void ledfxFillRGB(const U2 ix0, const U2 ix1, const U1 red, const U1 green, const U1 blue);

//! fill range of pixels given hue, saturation and value (HSV)
/*!
    \param[in] ix0  start index
    \param[in] ix1  end index
    \param[in] hue  hue value (0..255)
    \param[in] sat  saturation value (0..255)
    \param[in] val  (brightness) value (0..255)

    Setting both \c ix0 and \c ix1 to to 0 will fill all.
*/
void ledfxFillHSV(const U2 ix0, const U2 ix1, const U1 hue, const U1 sat, const U1 val);

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
U2 ledfxLimitCurrent(const U2 maPerLed, const U2 maMax, U2 *pMaUsed);

//@}


/* *************************************************************************** */
/*!
    \name Noise Effects

    \todo play with different noises: Worley, Simplex, ...

    \todo fireflys http://www.michaelfogleman.com/sync/

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
void ledfxNoiseRandom(const L init, const U2 ix0, const U2 ix1, const U2 num);

//! random distinct colour noise
/*!
    \param[in] init  set to \c true on first call to initialise things
    \param[in] ix0   start index
    \param[in] ix1   end index
    \param[in] num   number of LEDs to change colours in one call

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxNoiseRandomDistinct(const L init, const U2 ix0, const U2 ix1, const U2 num);

//! moving hue noise
/*!
    \param[in] init  set to \c true on first call to initialise things
    \param[in] ix0   start index
    \param[in] ix1   end index
    \param[in] num   number of LEDs to change colours in one call
    \param[in,out]   r0  effect state storage
    \param[in,out]   r1  effect state storage

    Setting both \c ix0 and \c ix1 to to 0 will apply the effect to all LEDs.
*/
void ledfxNoiseMovingHue(const L init, const U2 ix0, const U2 ix1, const U2 num, U1 *r0, U1 *r1);

//@}


/* *************************************************************************** */
/*!
    \name Colour Flow Effects
    \todo better plasma à la http://lodev.org/cgtutor/plasma.html, http://pastebin.com/MQhR526C,
          http://www.bidouille.org/prog/plasma

    @{
*/

//! concentric moving hue flow
/*!
    \todo make this centred for even numbers of rows/columns

    \param[in] init  set to \c true on first call to initialise things
    \param[in] step  how many hue steps to move per iteration
    \param[in,out]   r0  effect state storage
*/
void ledfxConcentricHueFlow(const L init, const I1 step, U1 *r0);

//! plasma effect
/*!
    \param[in] init  set to \c true on first call to initialise things
    \param[in,out]   r0  effect state storage

    The formulas used in this function are based on code floating the internet in various code
    (google "colorduino", "shiftbrite", et al.). While the original source is never properly
    referenced it all seems to be attributed to: copyright (c) 2011 Sam C. Lin, 2009 Ben Combee,
    2009 Ken Corey, 2008 Windell H. Oskay \todo find original reference for this
*/
void ledfxPlasma(const L init, R4 *r0);

//@}


/* *************************************************************************** */
/*!
    \name Matrix Only Effects

    \todo fire, water

    \todo game of life

    @{
*/

//@}


/* *************************************************************************** */

#endif // __LEDFX_H__

//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED effects (see \ref LEDFX)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    - Portions copyright by others (see ledfx.h)

    \addtogroup LEDFX
    @{
*/

#include <string.h>        // libc: string operations
#include <math.h>          // libc: mathematical functions

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "hw.h"            // ff: hardware
#include "debug.h"         // ff: debugging
#include "hsv2rgb.h"       // ff: HSV to RGV conversion
#include "ledfx.h"         // ff: LED effects

#if (FF_LEDFX_NUM_LED > 0)

/* ************************************************************************** */

// use fast math..
#define _sinf(x) hwMathFastSinf(x)
#define _cosf(x) hwMathFastCosf(x)
#define _sqrtf(x) hwMathFastSqrtf(x)

// check valid colour ordering
#if   (FF_LEDFX_ORDER == 123) // RGB
enum { _R_ = 0, _G_ = 1, _B_ = 2 };
#elif (FF_LEDFX_ORDER == 132) // RBG
enum { _R_ = 0, _B_ = 1, _G_ = 2 };
#elif (FF_LEDFX_ORDER == 213) // GRB
enum { _G_ = 0, _R_ = 1, _B_ = 2 };
#elif (FF_LEDFX_ORDER == 231) // GBR
enum { _G_ = 0, _B_ = 1, _R_ = 2 };
#elif (FF_LEDFX_ORDER == 312) // BRG
enum { _B_ = 0, _R_ = 1, _G_ = 2 };
#elif (FF_LEDFX_ORDER == 321) // BGR
enum { _B_ = 0, _G_ = 1, _R_ = 2 };
#else
#  error Illegal value for FF_LEDFX_ORDER
#endif

// LED matrix arrangement (keep in sync with docu above!)
#if   (FF_LEDFX_XY_ARR == 1)
#  define XY_TO_IX(x, y) ( ((y) * (FF_LEDFX_NUM_X)) + (x) )
#elif (FF_LEDFX_XY_ARR == 2)
#  define XY_TO_IX(x, y) ( (y) % 2 ? ( (((y) + 1) * (FF_LEDFX_NUM_X)) - (x) - 1 ) : ( ((y) * (FF_LEDFX_NUM_X)) + (x) ) )
#else
#  error Illegal value for FF_LEDFX_XY_ARR
#endif

// check consistent number of LED configuration
#if ((FF_LEDFX_NUM_LED) != ((FF_LEDFX_NUM_X) * (FF_LEDFX_NUM_Y)))
#  error FF_LEDFX_NUM_LED is not equal FF_LEDFX_NUM_X * FF_LEDFX_NUM_Y
#endif

// number of colour channels
#define _N_ 3

// the frame buffer
typedef union LEDFX_FB_u
{
    U1 ix[FF_LEDFX_NUM_LED][_N_];
    U1 raw [FF_LEDFX_NUM_LED * _N_];
} LEDFX_FB_t;

static LEDFX_FB_t sLedfxFrameBuf;

static U1 sLedfxBrightness;


/* ************************************************************************** */

void ledfxClear(const U2 ix0, const U2 ix1)
{
    if ( (ix0 == 0) && (ix1 == 0) )
    {
        memset(sLedfxFrameBuf.raw, 0, sizeof(sLedfxFrameBuf.raw));
    }
    else
    {
        const U2 _ix1 = MIN(ix1, FF_LEDFX_NUM_LED - 1);
        for (U2 ix = ix0; ix <= _ix1; ix++)
        {
            sLedfxFrameBuf.ix[ix][0] = 0;
            sLedfxFrameBuf.ix[ix][1] = 0;
            sLedfxFrameBuf.ix[ix][2] = 0;
        }
    }
}

inline U2 ledfxNumLeds(void)
{
    return FF_LEDFX_NUM_LED;
}

inline const U1 *ledfxGetFrameBuffer(void)
{
    return sLedfxFrameBuf.raw;
}

inline U2 ledfxGetFrameBufferSize(void)
{
    return (U2)sizeof(sLedfxFrameBuf.raw);
}

void ledfxSetBrightness(const U1 brightness)
{
    sLedfxBrightness = brightness < 255 ? brightness : 0;
}

static inline void sLedfxSetRGB(const U2 ix, const U1 red, const U1 green, const U1 blue)
{
    if (sLedfxBrightness)
    {
        const U4 thrs = 256 / sLedfxBrightness;
        const U4 brgt = sLedfxBrightness;
        if (red)
        {
            const U4 in = red;
            sLedfxFrameBuf.ix[ix][_R_] = (in <= thrs) ? 1 : ((in * brgt) >> 8);
        }
        else
        {
            sLedfxFrameBuf.ix[ix][_R_] = 0;
        }
        if (green)
        {
            const U4 in = green;
            sLedfxFrameBuf.ix[ix][_G_] = (in <= thrs) ? 1 : ((in * brgt) >> 8);
        }
        else
        {
            sLedfxFrameBuf.ix[ix][_G_] = 0;
        }
        if (blue)
        {
            const U4 in = blue;
            sLedfxFrameBuf.ix[ix][_B_] = (in <= thrs) ? 1 : ((in * brgt) >> 8);
        }
        else
        {
            sLedfxFrameBuf.ix[ix][_B_] = 0;
        }
    }
    else
    {
        sLedfxFrameBuf.ix[ix][_R_] = red;
        sLedfxFrameBuf.ix[ix][_G_] = green;
        sLedfxFrameBuf.ix[ix][_B_] = blue;
    }
}

inline void ledfxSetIxRGB(const U2 ix, const U1 red, const U1 green, const U1 blue) 
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        sLedfxSetRGB(ix, red, green, blue);
    }
}

inline void ledfxSetIxHSV(const U2 ix, const U1 hue, const U1 sat, const U1 val)
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        U1 red, green, blue;
        hsv2rgb(hue, sat, val, &red, &green, &blue);
        sLedfxSetRGB(ix, red, green, blue);
    }
}


void ledfxSetMatrixHSV(const U2 x, const U2 y, const U1 hue, const U1 sat, const U1 val)
{
    if ( (x < FF_LEDFX_NUM_X) && (y < FF_LEDFX_NUM_Y) )
    {
        const U2 ix = XY_TO_IX(x, y);
        U1 red, green, blue;
        hsv2rgb(hue, sat, val, &red, &green, &blue);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxSetMatrixRGB(const U2 x, const U2 y, const U1 red, const U1 green, const U1 blue)
{
    if ( (x < FF_LEDFX_NUM_X) && (y < FF_LEDFX_NUM_Y) )
    {
        const U2 ix = XY_TO_IX(x, y);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxFillRGB(const U2 ix0, const U2 ix1, const U1 red, const U1 green, const U1 blue)
{
    U2 _ix0 = ix0;
    U2 _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    for (U2 ix = _ix0; ix <= _ix1; ix++)
    {
        ledfxSetIxRGB(ix, red, green, blue);
    }
}
void ledfxFillHSV(const U2 ix0, const U2 ix1, const U1 hue, const U1 sat, const U1 val)
{
    U1 red, green, blue;
    hsv2rgb(hue, sat, val, &red, &green, &blue);
    ledfxFillRGB(ix0, ix1, red, green, blue);
}


// see ledfx.m for some real-world current measurements
U2 ledfxLimitCurrent(const U2 maPerLed, const U2 maMax, U2 *pMaUsed)
{
    const U4 maxVal = ( ((U4)maMax * _N_ * 256 * 10) + 5 ) / ( (U4)maPerLed * 10 );

    U4 usedVal = 0;
    U2 numLimit = 0;
    L limit = false;
    for (U2 ix = 0; ix < FF_LEDFX_NUM_LED; ix++)
    {
        if (!limit)
        {
            const U2 deltaVal = (U2)sLedfxFrameBuf.ix[ix][_R_]
                              + (U2)sLedfxFrameBuf.ix[ix][_G_]
                              + (U2)sLedfxFrameBuf.ix[ix][_B_];
            const U4 newVal = usedVal + deltaVal;
            if (newVal > maxVal)
            {
                limit = true;
            }
            else
            {
                usedVal = newVal;
            }
            //DEBUG("ix=%"F_U2" d=%"F_U2" n=%"F_U4" -> u=%"F_U4, ix, deltaVal, newVal, usedVal); hwTxFlush();
        }
        if (limit)
        {
            if (sLedfxFrameBuf.ix[ix][_R_] ||
                sLedfxFrameBuf.ix[ix][_G_] ||
                sLedfxFrameBuf.ix[ix][_B_])
            {
                sLedfxFrameBuf.ix[ix][_R_] = 1;
                sLedfxFrameBuf.ix[ix][_G_] = 1;
                sLedfxFrameBuf.ix[ix][_B_] = 1;
                usedVal += 3;
            }
            numLimit++;
        }
    }

    const U2 maUsed = ( (((1000 * usedVal) >> 8) * maPerLed) + 500 ) / (_N_ * 1000);
    if (pMaUsed)
    {
        *pMaUsed = maUsed;
    }

    //DEBUG("ledfx limiter: maPerLed=%"F_U2" maMax=%"F_U2" maxVal=%"F_U4" usedVal=%"F_U4" maUsed=%"F_U2" numLimit=%"F_U2"/%"F_U2,
    //    maPerLed, maMax, maxVal, usedVal, maUsed, numLimit, (U2)FF_LEDFX_NUM_LED);

    return numLimit;
}


/* ************************************************************************** */

void ledfxNoiseRandom(const L init, const U2 ix0, const U2 ix1, const U2 num)
{
    UNUSED(init);
    const U2 range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (U2 n = 0; n < num; n++)
    {
        const U4 rnd = hwMathGetRandom();
        const U2 ix = ix0 + (rnd % range);
        ledfxSetIxHSV(ix, rnd >> 8, 255, 255);
    }
}

void ledfxNoiseRandomDistinct(const L init, const U2 ix0, const U2 ix1, const U2 num)
{
    UNUSED(init);
    // \todo move to flash (PGM)
    const U1 hues[] = { 0, (1*255/6), (2*255/6), (3*255/6), (4*255/6), (5*255/6) };
    const U2 range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (U2 n = 0; n < num; n++)
    {
        const U4 rnd = hwMathGetRandom();
        const U2 ix = ix0 + (rnd % range);
        const U1 hue = hues[ (rnd >> 8) % NUMOF(hues) ];
        ledfxSetIxHSV(ix, hue, 255, 255);
    }
}

void ledfxNoiseMovingHue(const L init, const U2 ix0, const U2 ix1, const U2 num, U1 *r0, U1 *r1)
{
    if (init)
    {
        *r0 = 127;
        *r1 = 0;
    }

    U2 _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    ledfxFillHSV(ix0, _ix1, *r1, 255, 50);

    const U2 range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (U2 n = 0; n < num; n++)
    {
        const U4 rnd = hwMathGetRandom();
        const U2 ix = ix0 + (rnd % range);
        ledfxSetIxHSV(ix, *r1, 255, 255);
    }

    *r0 += 3;
    *r1 -= 7;
}

/* ************************************************************************** */

void ledfxConcentricHueFlow(const L init, const I1 step, U1 *r0)
{
    if (init)
    {
        *r0 = 0;
    }
    else
    {
        (*r0) += step;
    }

    const I2 x0 = FF_LEDFX_NUM_X / 2;
    const I2 y0 = FF_LEDFX_NUM_Y / 2;
    const I2 hueMax = 256/2;
    const U1 sat = 255;
    const U1 val = 255;
    I2 dX = x0 + 1;
    while (dX--)
    {
        I2 dY = y0 + 1;
        while (dY--)
        {
            const U1 hue = (U1)(((dX*dX) + (dY*dY))
               * hueMax / ((x0*x0) + (y0*y0))) + *r0;
            //DEBUG("ledfxConcentricHueFlow() %2i %2i %2i %2i %3u -> %2i %2i   %2i %2i  %2i %2i  %2i %2i",
            //      dx, dy, X0, Y0, hue,
            //      X0 + dx, dy, X0 - dx, dy, dx, Y0 + dy, dx, Y0 - dy);

            // FIXME: possible overflow?
            ledfxSetMatrixHSV(x0 + dX, y0 + dY, hue, sat, val);
            ledfxSetMatrixHSV(x0 - dX, y0 + dY, hue, sat, val);
            ledfxSetMatrixHSV(x0 + dX, y0 - dY, hue, sat, val);
            ledfxSetMatrixHSV(x0 - dX, y0 - dY, hue, sat, val);
        }
    }
}


static __INLINE R4 sDist(const R4 a, const R4 b, const R4 c, const R4 d)
{
    // based on: see header file
    const R4 cma = c - a;
    const R4 dmb = d - b;
    return _sqrtf( (cma * cma) + (dmb * dmb) );
}

void ledfxPlasma(const L init, R4 *r0)
{
    const U1 sat = 255;
    const U1 val = 255;

    if (init)
    {
        hwTic(0);
        // based on: see header file
        *r0 = 128000.0f; // pallete shift, FIXME: 128000, obviously.. :-/
        //for (U1 y = 0; y < MATRIX_NY; y++)
        //{
        //    for (U1 x = 0; x < MATRIX_NX; x++)
        //    {
        //        pState->plasma[x][y] = (U1)(
        //            256.0f + ( 256.0f * _sinf( (R4)x * (16.0f / 24.0f) ) ) +
        //            256.0f + ( 256.0f * _sinf( (R4)y * (8.0f / 16.0f) ) )
        //            ) / 2;
        //    }
        //}
        DEBUG("ledfxPlasma() init %"F_U4, hwToc(0)); // 15ms
    }

    hwTic(0);
    for (U2 y = 0; y < FF_LEDFX_NUM_Y; y++)
    {
        for (U2 x = 0; x < FF_LEDFX_NUM_X; x++)
        {
            // based on source: see progPlasma() docu
            const R4 value =
                _sinf( sDist((R4)x + *r0, y, 128.0f, 128.0) * (1.0f / 8.0f)) +
                _sinf( sDist(x, y, 64.0f, 64.0f) * (1.0f / 8.0f) ) +
                _sinf( sDist(x, (R4)y + (*r0 / 7.0f), 192.0f, 64.0f) * (1.0f / 7.0f) ) +
                _sinf( sDist(x, y, 192.0f, 100.0f) * (1.0 / 8.0) );
            const U1 hue = (U1)((U4)(value * 128.0) & 0xff);
            ledfxSetMatrixHSV(x, y, hue, sat, val);
        }
    }
    (*r0) -= 0.25; // smooth (the original code used -= 1)
    // AVR: ~45ms
    // ~250ms --> ~55ms
    DEBUG("ledfxPlasma() render %"F_U, hwToc(0));
}

void ledfxRainbow(const L init, const U2 ix0, const U2 ix1, U1 *r0)
{
    if (init)
    {
        *r0 = 0; // hue offset
    }
    else
    {
        (*r0)++;
    }
    const U1 sat = 255;
    const U1 val = 255;

    U2 _ix0 = ix0;
    U2 _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    for (U2 ix = _ix0; ix <= _ix1; ix++)
    {
        const U1 hue = (U4)(*r0) + ((U4)ix * 256 / 2 / FF_LEDFX_NUM_LED);
        //const U1 hue = *r0 + (((U2)ix * ((256 << 6) / FF_LEDFX_NUM_LED)) >> 6);
        ledfxSetIxHSV(ix, hue, sat, val);
    }

}


/* ************************************************************************** */

void ledfxRotor(const L init, R4 *r0, R4 *r1)
{
    const U1 sat = 255;
    const U1 val = 255;

    if (init)
    {
        *r0 = (R4)(hwMathGetRandom() & 0xff); // hue offset
        *r1 = 0.0f; // angle
    }
    else
    {
        *r0 += 0.25;
        *r1 += M_PI / 200;
    }

    // we rotate the coordinate system (or: a the line y = 0.5*x)
    hwTic(0);
    const R4 sinAngle = _sinf(*r1);
    const R4 cosAngle = _cosf(*r1);
    for (U2 y = 0; y < FF_LEDFX_NUM_Y; y++)
    {
        const R4 cosAngleY = cosAngle * ((R4)y +- ((R4)FF_LEDFX_NUM_Y/2.0f) + 0.5f);
        for (U2 x = 0; x < FF_LEDFX_NUM_X; x++)
        {
            const R4 sinAngleX = sinAngle * ((R4)x - ((R4)FF_LEDFX_NUM_X/2.0f) + 0.5f);

            // the distance of the point at (x,y) to the line is (dot product) (in [px])
            // [ x, y ] * [ sin(a), cos(a) ] = x * sin(a) + y * cos(a)
            const R4 dist = (sinAngleX + cosAngleY); // distance in [px]

            // (an approximation of) the maximum distance we'll see
            const R4 maxDist = 1.4142f * ((R4)FF_LEDFX_NUM_X + (R4)FF_LEDFX_NUM_Y) / 2.0f / 2.0f;

            // scale distance to the range -1..+1 (via sin() instead of linear)
            //const R4 normDist = _sinf( dist * (1.0f/maxDist) * (M_PI/2.0f));
            // scale distance to the range 0..+1 (symmetric) (via sin() instead of linear)
            //const R4 normDist = fabsf( _sinf( dist * (1.0f/maxDist) * (M_PI/2.0f)) );
            //const R4 normDist = fabsf( _cosf( dist * (1.0f/maxDist) * (M_PI/2.0f)) );
            const R4 normDist = fabsf( dist * (1.0f/maxDist) );

            // calculate hue
            const R4 hueSpan = 200.0f;
            const U1 hue = normDist * (hueSpan / 2.0) + (hueSpan / 2.0)
                // cycle through hue spectrum
                + *r0;

            //if (x == 0)
            //{
            //    DEBUG("y=%u dist=%.1f maxDist=%.1f normDist=%.1f h=%u", y, dist, maxDist, normDist, h);
            //}

            ledfxSetMatrixHSV(x, y, hue, sat, val);
        }
    }
    DEBUG("ledfxRotor() render %"F_U, hwToc(0));

}


/* ************************************************************************** */

/* ************************************************************************** */


#endif // (FF_LEDFX_NUM_LED > 0)

//@}
// eof

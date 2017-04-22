/*!
    \file
    \brief flipflip's Arduino Uno stuff: LED effects (see \ref LEDFX)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup LEDFX
    @{
*/

#include <string.h>        // libc: string operations

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

//
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

inline void ledfxSetRGB(const U2 ix, const U1 red, const U1 green, const U1 blue) 
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        sLedfxSetRGB(ix, red, green, blue);
    }
}

inline void ledfxSetHSV(const U2 ix, const U1 hue, const U1 sat, const U1 val)
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        U1 red, green, blue;
        hsv2rgb(hue, sat, val, &red, &green, &blue);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxFillRGB(const U2 ix0, const U2 ix1, const U1 red, const U1 green, const U1 blue)
{
    U2 _ix0 = ix0;
    U2 _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    for (U2 ix = _ix0; ix <= _ix1; ix++)
    {
        ledfxSetRGB(ix, red, green, blue);
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
        ledfxSetHSV(ix, rnd >> 8, 255, 255);
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
        ledfxSetHSV(ix, hue, 255, 255);
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
        ledfxSetHSV(ix, *r1, 255, 255);
    }

    *r0 += 3;
    *r1 -= 7;
}

/* ************************************************************************** */

/* ************************************************************************** */

/* ************************************************************************** */

/* ************************************************************************** */


#endif // (FF_LEDFX_NUM_LED > 0)

//@}
// eof

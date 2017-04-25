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
    uint8_t ix[FF_LEDFX_NUM_LED][_N_];
    uint8_t raw [FF_LEDFX_NUM_LED * _N_];
} LEDFX_FB_t;

static LEDFX_FB_t sLedfxFrameBuf;

static uint8_t sLedfxBrightness;


/* ************************************************************************** */

void ledfxClear(const uint16_t ix0, const uint16_t ix1)
{
    if ( (ix0 == 0) && (ix1 == 0) )
    {
        memset(sLedfxFrameBuf.raw, 0, sizeof(sLedfxFrameBuf.raw));
    }
    else
    {
        const uint16_t _ix1 = MIN(ix1, FF_LEDFX_NUM_LED - 1);
        for (uint16_t ix = ix0; ix <= _ix1; ix++)
        {
            sLedfxFrameBuf.ix[ix][0] = 0;
            sLedfxFrameBuf.ix[ix][1] = 0;
            sLedfxFrameBuf.ix[ix][2] = 0;
        }
    }
}

inline uint16_t ledfxNumLeds(void)
{
    return FF_LEDFX_NUM_LED;
}

inline const uint8_t *ledfxGetFrameBuffer(void)
{
    return sLedfxFrameBuf.raw;
}

inline uint16_t ledfxGetFrameBufferSize(void)
{
    return (uint16_t)sizeof(sLedfxFrameBuf.raw);
}

void ledfxSetBrightness(const uint8_t brightness)
{
    sLedfxBrightness = brightness < 255 ? brightness : 0;
}

static inline void sLedfxSetRGB(const uint16_t ix, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    if (sLedfxBrightness)
    {
        const uint32_t thrs = 256 / sLedfxBrightness;
        const uint32_t brgt = sLedfxBrightness;
        if (red)
        {
            const uint32_t in = red;
            sLedfxFrameBuf.ix[ix][_R_] = (in <= thrs) ? 1 : ((in * brgt) >> 8);
        }
        else
        {
            sLedfxFrameBuf.ix[ix][_R_] = 0;
        }
        if (green)
        {
            const uint32_t in = green;
            sLedfxFrameBuf.ix[ix][_G_] = (in <= thrs) ? 1 : ((in * brgt) >> 8);
        }
        else
        {
            sLedfxFrameBuf.ix[ix][_G_] = 0;
        }
        if (blue)
        {
            const uint32_t in = blue;
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

inline void ledfxSetIxRGB(const uint16_t ix, const uint8_t red, const uint8_t green, const uint8_t blue) 
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        sLedfxSetRGB(ix, red, green, blue);
    }
}

inline void ledfxSetIxHSV(const uint16_t ix, const uint8_t hue, const uint8_t sat, const uint8_t val)
{
    if (ix < FF_LEDFX_NUM_LED)
    {
        uint8_t red, green, blue;
        hsv2rgb(hue, sat, val, &red, &green, &blue);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxSetMatrixHSV(const uint16_t x, const uint16_t y, const uint8_t hue, const uint8_t sat, const uint8_t val)
{
    if ( (x < FF_LEDFX_NUM_X) && (y < FF_LEDFX_NUM_Y) )
    {
        const uint16_t ix = XY_TO_IX(x, y);
        uint8_t red, green, blue;
        hsv2rgb(hue, sat, val, &red, &green, &blue);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxSetMatrixRGB(const uint16_t x, const uint16_t y, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    if ( (x < FF_LEDFX_NUM_X) && (y < FF_LEDFX_NUM_Y) )
    {
        const uint16_t ix = XY_TO_IX(x, y);
        sLedfxSetRGB(ix, red, green, blue);
    }
}

void ledfxFillRGB(const uint16_t ix0, const uint16_t ix1, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    uint16_t _ix0 = ix0;
    uint16_t _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    for (uint16_t ix = _ix0; ix <= _ix1; ix++)
    {
        ledfxSetIxRGB(ix, red, green, blue);
    }
}
void ledfxFillHSV(const uint16_t ix0, const uint16_t ix1, const uint8_t hue, const uint8_t sat, const uint8_t val)
{
    uint8_t red, green, blue;
    hsv2rgb(hue, sat, val, &red, &green, &blue);
    ledfxFillRGB(ix0, ix1, red, green, blue);
}

// see ledfx.m for some real-world current measurements
uint16_t ledfxLimitCurrent(const uint16_t maPerLed, const uint16_t maMax, uint16_t *pMaUsed)
{
    const uint32_t maxVal = ( ((uint32_t)maMax * _N_ * 256 * 10) + 5 ) / ( (uint32_t)maPerLed * 10 );

    uint32_t usedVal = 0;
    uint16_t numLimit = 0;
    bool limit = false;
    for (uint16_t ix = 0; ix < FF_LEDFX_NUM_LED; ix++)
    {
        if (!limit)
        {
            const uint16_t deltaVal = (uint16_t)sLedfxFrameBuf.ix[ix][_R_]
                              + (uint16_t)sLedfxFrameBuf.ix[ix][_G_]
                              + (uint16_t)sLedfxFrameBuf.ix[ix][_B_];
            const uint32_t newVal = usedVal + deltaVal;
            if (newVal > maxVal)
            {
                limit = true;
            }
            else
            {
                usedVal = newVal;
            }
            //DEBUG("ix=%"PRIu16" d=%"PRIu16" n=%"PRIu32" -> u=%"PRIu32, ix, deltaVal, newVal, usedVal); hwTxFlush();
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

    const uint16_t maUsed = ( (((1000 * usedVal) >> 8) * maPerLed) + 500 ) / (_N_ * 1000);
    if (pMaUsed)
    {
        *pMaUsed = maUsed;
    }

    //DEBUG("ledfx limiter: maPerLed=%"PRIu16" maMax=%"PRIu16" maxVal=%"PRIu32" usedVal=%"PRIu32" maUsed=%"PRIu16" numLimit=%"PRIu16"/%"PRIu16,
    //    maPerLed, maMax, maxVal, usedVal, maUsed, numLimit, (uint16_t)FF_LEDFX_NUM_LED);

    return numLimit;
}


/* ************************************************************************** */

void ledfxNoiseRandom(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num)
{
    UNUSED(init);
    const uint16_t range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (uint16_t n = 0; n < num; n++)
    {
        const uint32_t rnd = hwMathGetRandom();
        const uint16_t ix = ix0 + (rnd % range);
        ledfxSetIxHSV(ix, rnd >> 8, 255, 255);
    }
}

void ledfxNoiseRandomDistinct(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num)
{
    UNUSED(init);
    // \todo move to flash (PGM)
    static const uint8_t hues[] PROGMEM = { 0, (1*255/6), (2*255/6), (3*255/6), (4*255/6), (5*255/6) };
    const uint16_t range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (uint16_t n = 0; n < num; n++)
    {
        const uint32_t rnd = hwMathGetRandom();
        const uint16_t ix = ix0 + (rnd % range);
        const uint8_t hue = pgm_read_byte( &hues[ (rnd >> 8) % NUMOF(hues) ] );
        ledfxSetIxHSV(ix, hue, 255, 255);
    }
}

void ledfxNoiseMovingHue(const bool init, const uint16_t ix0, const uint16_t ix1, const uint16_t num, uint8_t *r0, uint8_t *r1)
{
    if (init)
    {
        *r0 = 127;
        *r1 = 0;
    }

    uint16_t _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    ledfxFillHSV(ix0, _ix1, *r1, 255, 50);

    const uint16_t range = ( (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1 ) - ix0 + 1;
    for (uint16_t n = 0; n < num; n++)
    {
        const uint32_t rnd = hwMathGetRandom();
        const uint16_t ix = ix0 + (rnd % range);
        ledfxSetIxHSV(ix, *r1, 255, 255);
    }

    *r0 += 3;
    *r1 -= 7;
}

void ledfxStrobo(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0)
{
    if (init)
    {
        *r0 = 0;
    }
    else
    {
        (*r0)++;
    }
    if (*r0 % 2)
    {
        ledfxFillRGB(ix0, ix1, 255, 255, 255);
    }
    else
    {
        ledfxFillRGB(ix0, ix1, 0, 0, 0);
    }
}


/* ************************************************************************** */

void ledfxConcentricHueFlow(const bool init, const int8_t step, uint8_t *r0)
{
    if (init)
    {
        *r0 = 0;
    }
    else
    {
        (*r0) += step;
    }

    const int16_t x0 = FF_LEDFX_NUM_X / 2;
    const int16_t y0 = FF_LEDFX_NUM_Y / 2;
    const int16_t hueMax = 256/2;
    const uint8_t sat = 255;
    const uint8_t val = 255;
    int16_t dX = x0 + 1;
    while (dX--)
    {
        int16_t dY = y0 + 1;
        while (dY--)
        {
            const uint8_t hue = (uint8_t)(((dX*dX) + (dY*dY))
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

static inline float sDist(const float a, const float b, const float c, const float d)
{
    // based on: see header file
    const float cma = c - a;
    const float dmb = d - b;
    return _sqrtf( (cma * cma) + (dmb * dmb) );
}

void ledfxPlasma(const bool init, float *r0)
{
    const uint8_t sat = 255;
    const uint8_t val = 255;

    if (init)
    {
        hwTic(0);
        // based on: see header file
        *r0 = 128000.0f; // pallete shift, FIXME: 128000, obviously.. :-/
        //for (uint8_t y = 0; y < MATRIX_NY; y++)
        //{
        //    for (uint8_t x = 0; x < MATRIX_NX; x++)
        //    {
        //        pState->plasma[x][y] = (uint8_t)(
        //            256.0f + ( 256.0f * _sinf( (float)x * (16.0f / 24.0f) ) ) +
        //            256.0f + ( 256.0f * _sinf( (float)y * (8.0f / 16.0f) ) )
        //            ) / 2;
        //    }
        //}
        DEBUG("ledfxPlasma() init %"PRIu32, hwToc(0)); // 15ms
    }

    hwTic(0);
    for (uint16_t y = 0; y < FF_LEDFX_NUM_Y; y++)
    {
        for (uint16_t x = 0; x < FF_LEDFX_NUM_X; x++)
        {
            // based on source: see progPlasma() docu
            const float value =
                _sinf( sDist((float)x + *r0, y, 128.0f, 128.0) * (1.0f / 8.0f)) +
                _sinf( sDist(x, y, 64.0f, 64.0f) * (1.0f / 8.0f) ) +
                _sinf( sDist(x, (float)y + (*r0 / 7.0f), 192.0f, 64.0f) * (1.0f / 7.0f) ) +
                _sinf( sDist(x, y, 192.0f, 100.0f) * (1.0 / 8.0) );
            const uint8_t hue = (uint8_t)((uint32_t)(value * 128.0) & 0xff);
            ledfxSetMatrixHSV(x, y, hue, sat, val);
        }
    }
    (*r0) -= 0.25; // smooth (the original code used -= 1)
    // AVR: ~45ms
    // ~250ms --> ~55ms
    DEBUG("ledfxPlasma() render %"PRIu16, hwToc(0));
}

void ledfxRainbow(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0)
{
    if (init)
    {
        *r0 = 0; // hue offset
    }
    else
    {
        (*r0)++;
    }
    const uint8_t sat = 255;
    const uint8_t val = 255;

    uint16_t _ix0 = ix0;
    uint16_t _ix1 = (ix0 == 0) && (ix1 == 0) ? (FF_LEDFX_NUM_LED - 1) : ix1;
    for (uint16_t ix = _ix0; ix <= _ix1; ix++)
    {
        const uint8_t hue = (uint32_t)(*r0) + ((uint32_t)ix * 256 / 2 / FF_LEDFX_NUM_LED);
        //const uint8_t hue = *r0 + (((uint16_t)ix * ((256 << 6) / FF_LEDFX_NUM_LED)) >> 6);
        ledfxSetIxHSV(ix, hue, sat, val);
    }

}

void ledfxHueSweep(const bool init, const uint16_t ix0, const uint16_t ix1, uint8_t *r0)
{
    if (init)
    {
        *r0 = 0;
    }
    else
    {
        (*r0)++;
    }
    const uint8_t sat = 255;
    const uint8_t val = 255;
    ledfxFillHSV(ix0, ix1, *r0, sat, val);
}

void ledfxWaves(const bool init, uint8_t *r0, float *r1, float *r2)
{
    if (init)
    {
        *r0 = 0;    // hue
        *r1 = 0.0f; // time
        *r2 = 0.0f; // scale
    }
    const uint8_t sat = 255;
    for (uint16_t x = 0; x < FF_LEDFX_NUM_X; x++)
    {
        const float v = _sinf( ((float)x * _cosf(*r2)) + *r1 );
        for (uint16_t y = 0; y < FF_LEDFX_NUM_Y; y++)
        {
            ledfxSetMatrixHSV(x, y, *r0, sat, ( (v + 1.0f) * 125.0f ));
        }
    }

    // update hue
    (*r0)++;
    // update time
    *r1 += M_PI / ((float)FF_LEDFX_NUM_X * 10.0);
    *r1 = fmodf(*r1, 2.0f * M_PI);
    // update scale
    *r2 += M_PI / ((float)FF_LEDFX_NUM_X * 30.0f);
    *r2 = fmodf(*r2, 2.0f * M_PI);
}


/* ************************************************************************** */

void ledfxRotor(const bool init, float *r0, float *r1)
{
    const uint8_t sat = 255;
    const uint8_t val = 255;

    if (init)
    {
        *r0 = (float)(hwMathGetRandom() & 0xff); // hue offset
        *r1 = 0.0f; // angle
    }
    else
    {
        *r0 += 0.25;
        *r1 += M_PI / 200;
    }

    // we rotate the coordinate system (or: a the line y = 0.5*x)
    hwTic(0);
    const float sinAngle = _sinf(*r1);
    const float cosAngle = _cosf(*r1);
    for (uint16_t y = 0; y < FF_LEDFX_NUM_Y; y++)
    {
        const float cosAngleY = cosAngle * ((float)y +- ((float)FF_LEDFX_NUM_Y/2.0f) + 0.5f);
        for (uint16_t x = 0; x < FF_LEDFX_NUM_X; x++)
        {
            const float sinAngleX = sinAngle * ((float)x - ((float)FF_LEDFX_NUM_X/2.0f) + 0.5f);

            // the distance of the point at (x,y) to the line is (dot product) (in [px])
            // [ x, y ] * [ sin(a), cos(a) ] = x * sin(a) + y * cos(a)
            const float dist = (sinAngleX + cosAngleY); // distance in [px]

            // (an approximation of) the maximum distance we'll see
            const float maxDist = 1.4142f * ((float)FF_LEDFX_NUM_X + (float)FF_LEDFX_NUM_Y) / 2.0f / 2.0f;

            // scale distance to the range -1..+1 (via sin() instead of linear)
            //const float normDist = _sinf( dist * (1.0f/maxDist) * (M_PI/2.0f));
            // scale distance to the range 0..+1 (symmetric) (via sin() instead of linear)
            //const float normDist = fabsf( _sinf( dist * (1.0f/maxDist) * (M_PI/2.0f)) );
            //const float normDist = fabsf( _cosf( dist * (1.0f/maxDist) * (M_PI/2.0f)) );
            const float normDist = fabsf( dist * (1.0f/maxDist) );

            // calculate hue
            const float hueSpan = 200.0f;
            const uint8_t hue = normDist * (hueSpan / 2.0) + (hueSpan / 2.0)
                // cycle through hue spectrum
                + *r0;

            //if (x == 0)
            //{
            //    DEBUG("y=%u dist=%.1f maxDist=%.1f normDist=%.1f h=%u", y, dist, maxDist, normDist, h);
            //}

            ledfxSetMatrixHSV(x, y, hue, sat, val);
        }
    }
    DEBUG("ledfxRotor() render %"PRIu16, hwToc(0));

}

void ledfxRain(const bool init, LEDFX_RAIN_t *pState)
{
    if (init)
    {
        memset(pState, 0, sizeof(*pState));
    }

    if (pState->f <= 0)
    {
        pState->f = 100;
    }

    // clear all
    ledfxClear(0, 0);

    const uint8_t maxLen = 6;  // max length of the drop
    const uint8_t ds = 30;  // saturation steps
    const uint8_t dv = 20;  // value steps
    const uint8_t v0 = 255;

    // check all columns
    for (uint8_t x = 0; x < FF_LEDFX_NUM_X; x++)
    {
        // animate
        if (pState->pos[x])
        {
            const uint8_t hue = pState->hue[x];
            const int8_t p1 = pState->pos[x];
            const int8_t p0 = p1 - pState->len[x] + 1;
            uint8_t sat = 255 - (pState->len[x] * ds);
            uint8_t val = v0 - (pState->len[x] * dv);
            for (int8_t p = p0; p <= p1; p++)
            {
                sat += ds;
                val += dv;
                const int8_t y = FF_LEDFX_NUM_Y - p;
                if ( (y >= 0) && (y < FF_LEDFX_NUM_Y) )
                {
                    //DEBUG("prog: rain: %2i %2i %3u %3u %3u/%3u", p1, y, h, s, v, v0);
                    ledfxSetMatrixHSV(x, (uint8_t)y, hue, sat, val);
                }

            }

            //matrixSetXYHSV(x, MATRIX_NY - drops[x].pos, drops[x].hue, 255, drops[x].val);
            if (p0 > (FF_LEDFX_NUM_Y - 1))
            {
                pState->pos[x] = 0;
            }
            else
            {
                pState->pos[x]++;
            }
        }
        // maybe create a new raindrop
        else
        {
            const uint32_t rnd = hwMathGetRandom();
            if ((rnd % pState->f) == 0)
            {
                pState->pos[x] = 1;
                pState->hue[x] = rnd >> 8;
                pState->len[x] = ((rnd >> 16) % maxLen) + 1;
                pState->n++;
                //DEBUG("prog: rain: new %u %u f=%u n=%u", drops[x].hue, drops[x].len, pState->f, pState->n);
            }
        }
    }

    // reduce frequency
    if (pState->n >= 5)
    {
        pState->n = 0;
        if (pState->f > 50)
        {
            pState->f -= 10;
        }
        else if (pState->f > 20)
        {
            pState->f -= 5;
        }
        else
        {
            pState->f -= 1;
        }
    }
}

void ledfxStars(const bool init, LEDFX_STAR_t *pStars, const uint16_t nStars)
{
    const uint8_t vBase = 255 / 2;
    const uint8_t vMin = 3;

    // initialise
    if (init)
    {
        memset(pStars, 0, nStars * sizeof(*pStars));
        ledfxClear(0, 0);
    }

    // create stars as needed
    for (uint16_t starIx = 0; starIx < nStars; starIx++)
    {
        LEDFX_STAR_t *pStar = &pStars[starIx];

        // skip still active star
        if (pStar->speed)
        {
            continue;
        }

        // create new star at a random and free pixel
        bool uniq;
        do
        {
            uniq = TRUE;
            pStar->ix = hwMathGetRandom() % FF_LEDFX_NUM_LED;
            if (starIx > 0)
            {
                uint8_t testIx = starIx;
                while (testIx--)
                {
                    if (pStars[testIx].ix == pStar->ix)
                    {
                        uniq = FALSE;
                        break;
                    }
                }
            }
        } while (uniq == FALSE);
        const uint32_t rand = hwMathGetRandom();
        pStar->hue    = rand & 0xff;
        pStar->speed  = ((rand >> 8) % 5) + 1;
        pStar->valMax = vBase + ((rand >> 16) % vBase);
        pStar->val    = 1;

        DEBUG("ledfxStars() create %02"PRIu16": ix=%03"PRIu16" hue=%03"PRIu8" val=%03"PRIu8" valMax=%03"PRIu8" speed=%03"PRIi8,
            starIx, pStar->ix, pStar->hue, pStar->val, pStar->valMax, pStar->speed);

        break; // create one per iteration
    }

    // render and decay
    for (uint16_t starIx = 0; starIx < nStars; starIx++)
    {
        LEDFX_STAR_t *pStar = &pStars[starIx];

        // skip inactive stars (on start)
        if (pStar->speed == 0)
        {
            continue;
        }

        // render
        const uint8_t sat = 155 + (uint8_t)((uint32_t)100 * (uint32_t)pStar->val / (uint32_t)pStar->valMax);
        //DEBUG("ledfxStars() render starIx=%u/%u ix=%03u hue=%03u val=%03u valMax=%03u speed=%03i s=%03u", starIx, nStars,
        //      pStar->ix, pStar->hue, pStar->val, pStar->valMax, pStar->speed, s);

        ledfxSetIxHSV(pStar->ix, pStar->hue, sat, pStar->val);

        // calculate next
        const int16_t val = pStar->val + pStar->speed;
        if (pStar->speed > 0) // fade in
        {
            if (val <= (int16_t)pStar->valMax)
            {
                pStar->val = (uint8_t)val;
            }
            else
            {
                pStar->speed = -pStar->speed;
            }
        }
        else // fade out
        {
            if (val >= vMin)
            {
                pStar->val = (uint8_t)val;
            }
            else
            {
                pStar->val = vMin;
                pStar->speed = 0;
                ledfxSetIxRGB(pStar->ix, 0, 0, 0);
            }
        }

    }
}


/* ************************************************************************** */

/* ************************************************************************** */


#endif // (FF_LEDFX_NUM_LED > 0)

//@}
// eof

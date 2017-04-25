/*!
    \file
    \brief flipflip's Arduino Uno stuff: HSV to RGB conversion (see \ref HSV2RGB)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)
    - Portions copyright (c) 2010 kasperkamperman.com

    \addtogroup HSV2RGB
    @{
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "hsv2rgb.h"       // ff: template

/* ************************************************************************** */

#if (FF_HSV2RGB_METHOD == 2)
// saturation/value lookup table
// Saturation/Value lookup table to compensate for the nonlinearity of human
// vision.  Used in the getRGB function on saturation and brightness to make
// dimming look more natural. Exponential function used to create values below
// : x from 0 - 255 : y = round(pow( 2.0, x+64/40.0) - 1)  FIXME: this doesn't seem right
// From: http://www.kasperkamperman.com/blog/arduino/arduino-programming-hsb-to-rgb/

static const uint8_t skHsv2rgbDimCurve[256] =
{
      0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
      3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
      4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
      6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
      8,   8,   9,   9,   9,   9,   9,   9,  10,  10,  10,  10,  10,  11,  11,  11,
     11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
     15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
     20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
     27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
     36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
     48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
     63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
     83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99, 101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255
};
#endif // (FF_HSV2RGB_METHOD == 2)


inline void hsv2rgb(const uint8_t hue, const uint8_t sat, const uint8_t val, uint8_t *R, uint8_t *G, uint8_t *B)
{
#if (FF_HSV2RGB_METHOD == 1)
    const uint8_t Sat = sat;
    const uint8_t Val = val;
#elif (FF_HSV2RGB_METHOD == 2)
    const uint8_t Sat = 255 - skHsv2rgbDimCurve[255-sat];
    const uint8_t Val = skHsv2rgbDimCurve[val];
#else
#  error Illegal value for FF_HSV2RGB_METHOD!
#endif

    const uint32_t s = (6 * (uint32_t)hue) >> 8;               // the segment 0..5 (360/60 * [0..255] / 256)
    const uint32_t t = (6 * (uint32_t)hue) & 0xff;             // within the segment 0..255 (360/60 * [0..255] % 256)
    const uint32_t l = ((uint32_t)Val * (255 - (uint32_t)Sat)) >> 8; // lower level
    const uint32_t r = ((uint32_t)Val * (uint32_t)Sat * t) >> 16;    // ramp
    switch (s)
    {
        case 0: *R = (uint8_t)Val;        *G = (uint8_t)(l + r);    *B = (uint8_t)l;          break;
        case 1: *R = (uint8_t)(Val - r);  *G = (uint8_t)Val;        *B = (uint8_t)l;          break;
        case 2: *R = (uint8_t)l;          *G = (uint8_t)Val;        *B = (uint8_t)(l + r);    break;
        case 3: *R = (uint8_t)l;          *G = (uint8_t)(Val - r);  *B = (uint8_t)Val;        break;
        case 4: *R = (uint8_t)(l + r);    *G = (uint8_t)l;          *B = (uint8_t)Val;        break;
        case 5: *R = (uint8_t)Val;        *G = (uint8_t)l;          *B = (uint8_t)(Val - r);  break;
        default: *R = 0; *G = 0; *B = 0;
    }
}


/* ************************************************************************** */

//@}
// eof

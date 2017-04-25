/*!
    \file
    \brief flipflip's Arduino Uno stuff: HSV to RGB conversion (see \ref HSV2RGB)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup HSV2RGB HSV to RGB Conversion
    \ingroup FF

    This implements HSV to RGB routines.

    From [Wikipedia](http://en.wikipedia.org/wiki/File:HSV-RGB-comparison.svg):
    <img src="../HSV-RGB-comparison_svg_wikipedia.png"/>

    \todo study https://beesbuzz.biz/code/hsv_color_transforms.php

    @{
*/

#ifndef __HSV2RGB_H__
#define __HSV2RGB_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! convert HSV to RGB
/*!
    Configurable via #FF_HSV2RGB_METHOD.

    \param[in]  hue  hue value (scaled 0..255)
    \param[in]  sat  saturation value (scaled 0..255)
    \param[in]  val  "brightness" value (scaled 0..255)
    \param[out] R    red (0..255)
    \param[out] G    green (0..255)
    \param[out] B    blue (0..255)
*/
void hsv2rgb(const uint8_t hue, const uint8_t sat, const uint8_t val, uint8_t *R, uint8_t *G, uint8_t *B);


/* *************************************************************************** */

#endif // __HSV2RGB_H__

//@}
// eof

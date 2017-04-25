/*!
    \file
    \brief flipflip's Arduino Uno stuff: "ffchräis" LED rings application (see \ref PROJECTS_FFCHREIS)

    - Copyright (c) 2016-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup PROJECTS_FFCHREIS "ffchräis" LED Rings
    \ingroup PROJECTS

    [RGB LED Ring 8/12/16/24 Bit WS2812 5050 RGB LED with Integrated Driver](https://www.aliexpress.com/item/RGB-LED-Ring-8-12-16-24-Bit-WS2812-5050-RGB-LED-with-Integrated-Driver-Drop/32674368863.html)
    from the [MT Technology Co., Ltd.](https://www.aliexpress.com/store/535576) store on AliExpress:

    <img src="../../projects/ffchreis/ledring1.jpg"/><img src="../../projects/ffchreis/ledring2.jpg"/>

    See ffchreis.c source code for details.

    @{
*/

#ifndef __FFCHREIS_H__
#define __FFCHREIS_H__


#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */


void appInit(void);

void appCreateTask(void);


/* *************************************************************************** */

#endif // __FFCHREIS_H__
// @}
// eof

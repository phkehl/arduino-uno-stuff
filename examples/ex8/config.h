/*!
    \file
    \brief flipflip's Arduino Uno stuff: eighth example application (see \ref EXAMPLES_EX8)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX8
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_LEDFX_ORDER 132

#define FF_LEDFX_NUM_X 8
#define FF_LEDFX_NUM_Y 8
#define FF_LEDFX_NUM_LED (FF_LEDFX_NUM_X * FF_LEDFX_NUM_Y)

#define FF_ALIMATRIX_MODE 3

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      150

#define FF_OS_IDLE_STACK      100

#define FF_ROTENC_ENABLE      1
#define FF_ROTENC_PIN1        _D2
#define FF_ROTENC_PIN2        _D0
#define FF_ROTENC_PIN3        _D3

#define FF_HW_LOAD_PIN        _D9
#define FF_HW_TICK_PIN        _NONE

#define FF_LEDFX_NUM_LED      200
#define FF_LEDFX_ORDER        123



#define DL_D0_PIN   _D4
#define DL_D1_PIN   _D5
#define DL_D2_PIN   _D6
#define DL_D3_PIN   _D7
#define DL_D4_PIN   _A0
#define DL_D5_PIN   _A1
#define DL_D6_PIN   _A2
#define DL_A0_PIN   _A3
#define DL_A1_PIN   _A4
#define DL_WR_PIN   _A5
#define DL_BL_PIN   _D8
#define WS2812_PIN  _D12


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

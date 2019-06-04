/*!
    \file
    \brief flipflip's Arduino Uno stuff: Geiger Counter display (see \ref PROJECTS_GEIGERDISP)

    - Copyright (c) 2019 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_GEIGERDISP
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_HW_LOAD_PIN        _NONE // _D13
#define FF_HW_TICK_PIN        _NONE // _A1

#define FF_HW_RXTX_BAUDRATE   9600
#define FF_HW_RX_BUFSIZE      255
#define FF_HW_TX_BUFSIZE      255

#define FF_OS_IDLE_STACK      150
#define FF_SYS_TASK_STACK     350

#define FF_SYS_MON_STRLEN     128


#define DL_D0_PIN   _D2
#define DL_D1_PIN   _D3
#define DL_D2_PIN   _D4
#define DL_D3_PIN   _D5
#define DL_D4_PIN   _D8
#define DL_D5_PIN   _D7
#define DL_D6_PIN   _D6
#define DL_A0_PIN   _A3
#define DL_A1_PIN   _A2
#define DL_WR_PIN   _A1
#define DL_BL_PIN   _D9

#define DL_CE1_PIN  _D10
#define DL_CE2_PIN  _D11
#define DL_CLR_PIN  _D12
#define DL_CUE_PIN  _D13
#define DL_CU_PIN   _A0

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

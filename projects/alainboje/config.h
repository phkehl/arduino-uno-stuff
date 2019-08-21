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


#define FF_HW_RXTX_BAUDRATE   115200
#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      255

#define FF_OS_IDLE_STACK      150
#define FF_SYS_TASK_STACK     350

#define FF_SYS_MON_STRLEN     128

#define FF_HSV2RGB_METHOD 2
#define FF_LEDFX_NUM_LED 3
#define FF_LEDFX_ORDER 123

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

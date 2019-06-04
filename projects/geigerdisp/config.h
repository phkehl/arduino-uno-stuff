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

#define FF_HW_LOAD_PIN        _D13

#define FF_HW_RXTX_BAUDRATE   9600
#define FF_HW_RX_BUFSIZE      255
#define FF_HW_TX_BUFSIZE      255

#define FF_OS_IDLE_STACK      150
#define FF_SYS_TASK_STACK     350

#define FF_SYS_MON_STRLEN     128

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

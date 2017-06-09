/*!
    \file
    \brief flipflip's Arduino Uno stuff: eighth example application (see \ref EXAMPLES_EX09)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX09
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_HW_LOAD_PIN        _D13

#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      255
#define FF_OS_IDLE_STACK      150
#define FF_SYS_TASK_STACK     350


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip phone (see \ref PROJECTS_FFPHONE)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define ARF32_RESET_PIN       _D10
#define ARF32_RX_PIN          _D19 // PD2, UART1!
#define ARF32_TX_PIN          _D18 // PD3, UART1!

#define AG1170_PD_PIN         _D9  // power-down (out)
#define AG1170_FR_PIN         _D5  // forward/reverse (out)
#define AG1170_RM_PIN         _D6  // ring mode (out)
#define AG1170_SHK_PIN        _D7  // switch hook (in)

#define FF_HW_LOAD_PIN        _D13

#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      255
#define FF_OS_IDLE_STACK      150
#define FF_SYS_TASK_STACK     350

#define FF_HD44780_RS_PIN     _D22
#define FF_HD44780_RW_PIN     _D23
#define FF_HD44780_E_PIN      _D24
#define FF_HD44780_D4_PIN     _D25
#define FF_HD44780_D5_PIN     _D26
#define FF_HD44780_D6_PIN     _D27
#define FF_HD44780_D7_PIN     _D28

#define FF_SYS_MON_STRLEN     128

// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

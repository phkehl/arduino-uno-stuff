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

#define ARF32_RESET_PIN       _D10
#define ARF32_RX_PIN          _D19 // PD2, UART1!
#define ARF32_TX_PIN          _D18 // PD3, UART1!

#define AG1170_PD_PIN         _D9
#define AG1170_FR_PIN         _D2
#define AG1170_RM_PIN         _D3
#define AG1170_SHK_PIN        _D4

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


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

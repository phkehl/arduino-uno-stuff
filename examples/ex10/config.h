/*!
    \file
    \brief flipflip's Arduino Uno stuff: tenth example application (see \ref EXAMPLES_EX10)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX10
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_HW_LOAD_PIN _D13

#define FF_HD44780_RS_PIN  _D22
#define FF_HD44780_RW_PIN  _D23
#define FF_HD44780_E_PIN   _D24
#define FF_HD44780_D4_PIN  _D25
#define FF_HD44780_D5_PIN  _D26
#define FF_HD44780_D6_PIN  _D27
#define FF_HD44780_D7_PIN  _D28


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

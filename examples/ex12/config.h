/*!
    \file
    \brief flipflip's Arduino Uno stuff: twelfth example application (see \ref EXAMPLES_EX12)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX12
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */

#define FF_HW_NUM_TICTOC      1

#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      200
#define FF_OS_IDLE_STACK      100

#define FF_ROTENC_ENABLE      1
#define FF_ROTENC_PIN1        _D2
#define FF_ROTENC_PIN2        _D0
#define FF_ROTENC_PIN3        _D3


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

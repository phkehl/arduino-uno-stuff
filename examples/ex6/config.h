/*!
    \file
    \brief flipflip's Arduino Uno stuff: fifth example application (see \ref EXAMPLES_EX5)

    - Copyright (c) 2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX5
    @{
*/

#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/* *************************************************************************** */


//  LED for the system load and status
#define FF_HW_LOAD_PIN _PD4

// system tick signal
#define FF_HW_TICK_PIN _PD5

// serial port input buffer size
#define FF_HW_RX_BUFSIZE 125

// use all UBX messages
#define FF_UBX_NAV_PVT_USE       1
#define FF_UBX_NAV_AOPSTATUS_USE 1
#define FF_UBX_NAV_CLOCK_USE     1
#define FF_UBX_NAV_POSECEF_USE   1
#define FF_UBX_NAV_VELECEF_USE   1
#define FF_UBX_NAV_STATUS_USE    1
#define FF_UBX_INF_ERROR_USE     2
#define FF_UBX_INF_WARNING_USE   2
#define FF_UBX_INF_NOTICE_USE    2
#define FF_UBX_INF_TEST_USE      1
#define FF_UBX_INF_DEBUG_USE     1



// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __APP_CONFIG_H__
//@}
// eof

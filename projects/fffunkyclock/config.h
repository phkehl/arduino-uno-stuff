/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's funky clock (see \ref PROJECTS_FFFUNKYCLOCK)

    - Copyright (c) 2014-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFFUNKYCLOCK
    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


// serial port input and output buffer sizes
#define FF_HW_RX_BUFSIZE 50
#define FF_HW_TX_BUFSIZE 250

// use some UBX messages
#define FF_UBX_NAV_PVT_USE       1 // this is all we really need...
#define FF_UBX_NAV_AOPSTATUS_USE 0
#define FF_UBX_NAV_CLOCK_USE     0
#define FF_UBX_NAV_POSECEF_USE   0
#define FF_UBX_NAV_VELECEF_USE   0
#define FF_UBX_NAV_STATUS_USE    1
#define FF_UBX_INF_ERROR_USE     2
#define FF_UBX_INF_WARNING_USE   2
#define FF_UBX_INF_NOTICE_USE    2
#define FF_UBX_INF_TEST_USE      0
#define FF_UBX_INF_DEBUG_USE     0

// we have this many LEDs
#define FF_LEDFX_NUM_LED (2*7*4)

// GRB colour ordering
#define FF_LEDFX_ORDER 123


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

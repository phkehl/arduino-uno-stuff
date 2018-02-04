/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip clock (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER

    \ref ARDUINOPINS usage:
    - D0: \ref ROTENC pin 2
    - D1: serial tx for \ref DEBUG output
    - D2: \ref ROTENC pin 1 ("CLK")
    - D3: \ref ROTENC pin 3 ("SW")
    - D4: Siemens DL2416T display: data input (D0)
    - D5: Siemens DL2416T display: data input (D1)
    - D6: Siemens DL2416T display: data input (D2)
    - D7: Siemens DL2416T display: data input (D3)
    - D8: Siemens DL2416T display: display blank (~BL)
    - D9: \ref FF_HW_LOAD_PIN
    - D10 (SS): needed by \ref WS2801 (FIXME: really?)
    - D11 (MOSI): \ref WS2801 data ("DI")
    - D12: \ref WS2812 data pin
    - D13 (SCK): \ref WS2801 clock output ("CI")
    - A0: Siemens DL2416T display: data input (D4)
    - A1: Siemens DL2416T display: data input (D5)
    - A2: Siemens DL2416T display: data input (D6)
    - A3: Siemens DL2416T display: write (~WR)
    - A4: Siemens DL2416T display: address (A1)
    - A5: Siemens DL2416T display: address (A0)

    See also config.h.

    @{
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* *************************************************************************** */


#define FF_HW_RX_BUFSIZE      0
#define FF_HW_TX_BUFSIZE      150

#define FF_OS_IDLE_STACK      100

#define FF_ROTENC_ENABLE      1
#define FF_ROTENC_PIN1        _D2
#define FF_ROTENC_PIN2        _D0
#define FF_ROTENC_PIN3        _D3

#define FF_HW_LOAD_PIN        _D9
#define FF_HW_TICK_PIN        _NONE

#define FF_LEDFX_NUM_LED      200
#define FF_LEDFX_ORDER        123 // must be 123



#define DL_D0_PIN   _D4
#define DL_D1_PIN   _D5
#define DL_D2_PIN   _D6
#define DL_D3_PIN   _D7
#define DL_D4_PIN   _A0
#define DL_D5_PIN   _A1
#define DL_D6_PIN   _A2
#define DL_A0_PIN   _A5
#define DL_A1_PIN   _A4
#define DL_WR_PIN   _A3
#define DL_BL_PIN   _D8
#define WS2812_PIN  _D12


// complete configuration by including the defaults
#include "defconfig.h"


/* *************************************************************************** */

#endif // __CONFIG_H__

//@}
// eof

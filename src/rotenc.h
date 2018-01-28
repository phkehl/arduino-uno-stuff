/*!
    \file
    \brief flipflip's Arduino Uno stuff: rotary encoder input (see \ref ROTENC)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup ROTENC Rotary Encoder Input
    \ingroup FF

    This implements support for a rotary encoder input device. It works for rotary encoder input
    modules with included push buttons. When used, it uses external interrupt 0 and 1 (and the
    corresponding pins) and a third selectable pin.

    <img src="../../src/rotenc.jpg"/>

    Configuration:
    - #FF_ROTENC_ENABLE
    - #FF_ROTENC_PIN1 ("CLK" in the picture above)
    - #FF_ROTENC_PIN2 ("DT" in the picture above)
    - #FF_ROTENC_PIN3 ("SW" in the picture above)
    - #FF_ROTENC_NUMQ

    @{
*/

#ifndef __ROTENC_H__
#define __ROTENC_H__

#include "stdstuff.h"      // ff: useful macros and types


/* *************************************************************************** */

//! initialise rotary encoder
void rotencInit(void);

//! events from the rotary encoder
typedef enum ROTENC_EVENT_e
{
    ROTENC_NONE,       //!< no event
    ROTENC_INC,        //!< increment
    ROTENC_DEC,        //!< decrement
    ROTENC_INC_DN,     //!< increment with button down
    ROTENC_DEC_DN,     //!< decrement with button down
    ROTENC_BTN,        //!< button pressed
    ROTENC_BTN_LONG,   //!< button long press
} ROTENC_EVENT_t;

//! get next event
/*!
    \param[in] timeout  maximum time to wait for an event (0 for endless, -1 return immediately)

    \returns the next event (#ROTENC_NONE if there was no action)

    \note Up to #FF_ROTENC_NUMQ events are queued. So call this often enough to catch/process them
          all.
*/
ROTENC_EVENT_t rotencGetEvent(int32_t timeout);

//! clear event queue
void rotencClearEvents(void);


/* *************************************************************************** */

#endif // __ROTENC_H__

//@}
// eof

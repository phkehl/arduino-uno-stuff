/*!
    \file
    \brief flipflip's Arduino Uno stuff: rotary encoder input (see \ref ROTENC)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup ROTENC
    @{
*/

#include <avr/io.h>        // avr: AVR device-specific IO definitions
#include <avr/interrupt.h> // avr: interrupt things

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system abstractions
#include "rotenc.h"        // ff: rotary encoder input

/* ************************************************************************** */

#if (FF_ROTENC_ENABLE > 0)

// check config
#if (!PIN_IS_SAME(FF_ROTENC_PIN1, _PD2))
#  error Illegal value for FF_ROTENC_PIN1! The current implementation can only use _PD2 (aka _D2).
#endif
#if (!PIN_IS_SAME(FF_ROTENC_PIN3, _PD3))
#  error Illegal value for FF_ROTENC_PIN3! The current implementation can only use _PD3 (aka _D3).
#endif

//  clock-wise (increment):                  anti clock-wise (decrement):
//
//           1         2                          1         2
//   --------+         +---------             ----+         +-------------
//           |<...d...>|                          |<...d...>|
//           |         |                          |         |
//           +---------+                          +---------+
//                     .                                    .
//   ----+         +---o---------             --------+     .   +---------
//       |         |                                  |     .   |
//       |         |                                  |     .   |
//       +---------+                                  +-----o---+
//
//
//  1 = ISR fires as we start rotating (falling edge)
//  2 = ISR fires again as it locks into the next position (raising edge)
//  d = we only accept the reading if d > ROTENC_MIN_DN_TIME
//  o = if it is, we sample the second signal here to determine the direction
//

#define ROTENC_ROT_THRS       3
#define ROTENC_BTN_THRS      15 // minumum time button must be pressed down
#define ROTENC_LONG_THRS    550 // minumum time for long press


static OS_QUEUE_t sRotencEventQueue;
static volatile uint32_t svRotencRotMsss;
static volatile uint32_t svRotencBtnMsss;
static volatile bool svRotencBtnDn;
static OS_TIMER_t svRotencBtnTimer;

// external interrupt 0 handles rotary encoder signal
ISR(INT0_vect)
{
    osIsrEnter();

    // down
    if (!PIN_GET(FF_ROTENC_PIN1))
    {
        svRotencRotMsss = osTaskGetTicks();
    }
    // up (snap in)
    else
    {
        if (svRotencRotMsss)
        {
            const uint32_t dt = osTaskGetTicks() - svRotencRotMsss;
            if (dt > ROTENC_ROT_THRS)
            {
                const bool isInc = PIN_GET(FF_ROTENC_PIN2);
                ROTENC_EVENT_t ev;
                if (svRotencBtnDn)
                {
                    ev = isInc ? ROTENC_INC_DN : ROTENC_DEC_DN;
                    svRotencBtnMsss = 0; // invalidate button press
                }
                else
                {
                    ev = isInc ? ROTENC_INC : ROTENC_DEC;
                }
                osQueueSend(&sRotencEventQueue, &ev, -1);
            }
        }
        svRotencRotMsss = 0;
    }

    osIsrLeave();
}

static void sRotencBtnTimerCb(void *pArg);

// external interrupt 1 handles button presses
ISR(INT1_vect)
{
    osIsrEnter();

    // button down
    if (!PIN_GET(FF_ROTENC_PIN3))
    {
        svRotencBtnMsss = osTaskGetTicks();
        svRotencBtnDn = true;
        osTimerKill(&svRotencBtnTimer);
        osTimerArm(&svRotencBtnTimer, sRotencBtnTimerCb, NULL, ROTENC_LONG_THRS, 0);
    }
    // button up
    else
    {
        if (svRotencBtnMsss)
        {
            if ((osTaskGetTicks() - svRotencBtnMsss) > ROTENC_BTN_THRS)
            {
                const ROTENC_EVENT_t ev = ROTENC_BTN;
                osQueueSend(&sRotencEventQueue, &ev, -1);
            }
        }
        svRotencBtnMsss = 0;
        svRotencBtnDn = false;
        osTimerKill(&svRotencBtnTimer);
    }

    osIsrLeave();
}

static void sRotencBtnTimerCb(void *pArg)
{
    UNUSED(pArg);

    // button still active and down
    if (svRotencBtnMsss && !PIN_GET(FF_ROTENC_PIN3))
    {
        const ROTENC_EVENT_t ev = ROTENC_BTN_LONG;
        osQueueSend(&sRotencEventQueue, &ev, -1);
        svRotencBtnMsss = 0;
        svRotencBtnDn = false; // invalidate
    }
}

const char *rotencEventStr(const ROTENC_EVENT_t event)
{
    switch (event)
    {
        case ROTENC_NONE:      return PSTR("NONE");
        case ROTENC_INC:       return PSTR("INC");
        case ROTENC_DEC:       return PSTR("DEC");
        case ROTENC_INC_DN:    return PSTR("INC_DN");
        case ROTENC_DEC_DN:    return PSTR("DEC_DN");
        case ROTENC_BTN:       return PSTR("BTN");
        case ROTENC_BTN_LONG:  return PSTR("BTN_LONG");
        default:               return PSTR("???");
    }
}


// -------------------------------------------------------------------------------------------------

void rotencInit(void)
{
    DEBUG("rotenc: init");

    // event queue
    static ROTENC_EVENT_t sRotencQueueBuf[FF_ROTENC_NUMQ];
    osQueueCreate(&sRotencEventQueue, &sRotencQueueBuf, NUMOF(sRotencQueueBuf), sizeof(ROTENC_EVENT_t));

    // rotary encoder
    PIN_INPUT(FF_ROTENC_PIN1);         // signals
    PIN_PULLUP_ON(FF_ROTENC_PIN1);
    PIN_INPUT(FF_ROTENC_PIN2);
    PIN_PULLUP_ON(FF_ROTENC_PIN2);
    CLRBITS(EICRA, BIT(ISC01));        // any change in logic level..
    SETBITS(EICRA, BIT(ISC00));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT0));         // enable INT0
    SETBITS(EIFR, BIT(INTF0));         // clear INT0

    // button
    PIN_INPUT(FF_ROTENC_PIN3);
    PIN_PULLUP_ON(FF_ROTENC_PIN3);
    CLRBITS(EICRA, BIT(ISC11));        // any change in logic level..
    SETBITS(EICRA, BIT(ISC10));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT1));         // enable INT1
    SETBITS(EIFR, BIT(INTF1));         // clear INT1
}

ROTENC_EVENT_t rotencGetEvent(int32_t timeout)
{
    ROTENC_EVENT_t ev;
    if (!osQueueReceive(&sRotencEventQueue, &ev, timeout))
    {
        ev = ROTENC_NONE;
    }
    return ev;
}

void rotencClearEvents(void)
{
    osQueueClear(&sRotencEventQueue);
}

#endif // (FF_ROTENC_ENABLE > 0)

/* ************************************************************************** */

//@}
// eof

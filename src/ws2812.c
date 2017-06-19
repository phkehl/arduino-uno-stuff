/*!
    \file
    \brief flipflip's Arduino Uno stuff: WS2812 LED driver (see \ref WS2812)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)
    - Portions copyright (c) 2014 by "Josh" (http://josh.com, see below)

    \addtogroup WS2812
    @{

    \todo Use osTask{Suspend,Resume}Scheduler() instead of CS_{ENTER,LEAVE} and only put the
          critical bit of in a CS (see josh.com's example). Consider that in TIMER0_COMPA_vect and
          only count up system ticks but don't run timers or schedule tasks.
*/

#include <light_ws2812.h>  // Tim's low-level WS2812 driver

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "os.h"            // ff: operating system abstractions
#include "ws2812.h"        // ff: WS2812 LED driver

#include "debug.h"

/* ************************************************************************** */

/*
    - signal timing
      - 0 bit: 400ns high, 850ns low (=1.25us)
      - 1 bit: 800ns high, 450ns low (=1.25us)
      - tolerance: 150ns
      - F_CPU=8MHz: 125ns/clock
        - 0 bit: 2.7 + 6.8 [clocks] --> 3 + 7
        - 1 bit: 6.4 + 3.6 [clocks] --> 7 + 3
      - F_CPU=16MHz: 62.5ns/clock
        - 0 bit: 6.4 + 13.6 [clocks]
        - 1 bit: 12.8 + 7.2 [clocks]

    - total time
      - 24 + 16 + 12 + 8 + 1 = 61 LEDs
      - 61 * 3 * 8 * 1.25us = 610us = 1.83ms
    - measured:
      - 1.99ms, 250ns + 1000ns (1375ns) / 875ns + 375ns (750ns)
*/

void _ws2812Send(volatile uint8_t *pPort, volatile uint8_t *pDdr, uint8_t mask, const uint8_t *data, const uint8_t size)
{
    // enable output
    *pDdr |= mask;

    CS_ENTER;
    if (pPort == &PORTB)
    {
        //DEBUG("PORTB");
        light_ws2812_send_PORTB(data, size, mask);
    }
    else if (pPort == &PORTC)
    {
        //DEBUG("PORTC");
        light_ws2812_send_PORTC(data, size, mask);
    }
    else if (pPort == &PORTD)
    {
        //DEBUG("PORTD");
        light_ws2812_send_PORTD(data, size, mask);
    }
    CS_LEAVE;
}


/* ************************************************************************** */

//@}
// eof

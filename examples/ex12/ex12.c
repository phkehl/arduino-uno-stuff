/*!
    \file
    \brief flipflip's Arduino Uno stuff: twelfth example application (see \ref EXAMPLES_EX12)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX12

    The interrupt pin should be debounced (100nF or so between pin and ground) to minimise ISR load.

    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "ex12.h"

/* ***** events ************************************************************** */

typedef enum EVENT_e
{
    EVENT_CNT_INC,
    EVENT_CNT_DEC,
    EVENT_BTN,
} EVENT_t;

#define EVENT_QUEUE_LEN 10

static OS_QUEUE_t sEventQueue;

static volatile uint32_t svEventLastMsss;

#define EVENT_MIN_DT 20


/* ***** rotary encoder ****************************************************** */

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
//  o = if it is, we sample the second signal here and determine the direction
//

#define ROTENC_PIN1 _PD2 // pin 1 (must be PD2 = INT0)
#define ROTENC_PIN2 _PD4 // pin 2

static volatile uint32_t svRotEncDnMsss;

#define ROTENC_MIN_DN_TIME 5

ISR(INT0_vect) // external interrupt 0
{
    osIsrEnter();

    // down
    if (!PIN_GET(ROTENC_PIN1))
    {
        svRotEncDnMsss = osTaskGetTicks();
    }
    // up (snap in)
    else
    {
        const uint32_t msss = osTaskGetTicks();
        if (svRotEncDnMsss && ((msss - svRotEncDnMsss) > ROTENC_MIN_DN_TIME))
        {
            const EVENT_t ev = PIN_GET(ROTENC_PIN2) ? EVENT_CNT_INC : EVENT_CNT_DEC;
            osQueueSend(&sEventQueue, &ev, -1);
        }
        svRotEncDnMsss = 0;
    }

    osIsrLeave();
}


/* ***** button ************************************************************** */

#define BUTTON_PIN  _PD3 // (must be PD3 = INT1)

#define BUTTON_MIN_DN_TIME 20 // minumum time button must be pressed down

static volatile uint32_t svButtonDnMsss;

ISR(INT1_vect) // external interrupt 1
{
    osIsrEnter();

    // button down
    if (!PIN_GET(BUTTON_PIN))
    {
        svButtonDnMsss = osTaskGetTicks();
    }
    // button up
    else
    {
        const uint32_t msss = osTaskGetTicks();
        if (svButtonDnMsss && ((msss - svButtonDnMsss) > BUTTON_MIN_DN_TIME))
        {
            const EVENT_t ev = EVENT_BTN;
            osQueueSend(&sEventQueue, &ev, -1);
        }
        svButtonDnMsss = 0;
    }

    osIsrLeave();
}



/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex12: init");

    // event queue
    static EVENT_t sEventQueueBuf[EVENT_QUEUE_LEN];
    osQueueCreate(&sEventQueue, &sEventQueueBuf, EVENT_QUEUE_LEN, sizeof(EVENT_t));

    // rotary encoder
    PIN_INPUT(ROTENC_PIN1);            // signals
    PIN_PULLUP_ON(ROTENC_PIN1);
    PIN_INPUT(ROTENC_PIN2);
    PIN_PULLUP_ON(ROTENC_PIN2);
    CLRBITS(EICRA, BIT(ISC01));        // any change in logic level..
    SETBITS(EICRA, BIT(ISC00));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT0));         // enable INT0
    SETBITS(EIFR, BIT(INTF0));         // clear INT0

    // button
    PIN_INPUT(BUTTON_PIN);
    PIN_PULLUP_ON(BUTTON_PIN);
    CLRBITS(EICRA, BIT(ISC11));        // any change in logic level..
    SETBITS(EICRA, BIT(ISC10));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT1));         // enable INT1
    SETBITS(EIFR, BIT(INTF1));         // clear INT1

    // register status function for the system task
    sysRegisterMonFunc(sAppStatus);
}

// starts the user application task
void appCreateTask(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("app", 5, &task, stack, sizeof(stack), sAppTask, NULL);
}

/* ***** application task **************************************************** */


// application task
static void sAppTask(void *pArg)
{
    // not using the task argument
    UNUSED(pArg);

    // clear event queue
    osQueueClear(&sEventQueue);

    static int16_t count;

    while (ENDLESS)
    {
        EVENT_t ev;
        if (osQueueReceive(&sEventQueue, &ev, 1000))
        {
            switch (ev)
            {
                case EVENT_CNT_INC:
                    DEBUG("INC %"PRIu16, count);
                    count++;
                    break;
                case EVENT_CNT_DEC:
                    DEBUG("DEC %"PRIu16, count);
                    count--;
                    break;
                case EVENT_BTN:
                    DEBUG("BTN");
                    break;
            }
        }
    }
}


/* ***** application status ************************************************** */

// make application status string
static void sAppStatus(char *str, const size_t size)
{
    /*const int n = */snprintf_P(str, size, PSTR("status..."));
}


//@}
// eof

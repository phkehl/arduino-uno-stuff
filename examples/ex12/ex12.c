/*!
    \file
    \brief flipflip's Arduino Uno stuff: twelfth example application (see \ref EXAMPLES_EX12)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup EXAMPLES_EX12

    It assumes an encoder with detents and hence there is no resolution to loos when it only uses
    the falling edge of one pin and sampling the other pin. The interrupt pin has to be debounced
    (100nF or so between pin and ground).

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
    EVENT_SGL_PRESS,
    EVENT_DBL_PRESS,
    EVENT_LNG_PRESS,
} EVENT_t;

#define EVENT_QUEUE_LEN 10

static OS_QUEUE_t sEventQueue;


/* ***** rotary encoder ****************************************************** */

#define ROTENC_PIN1 _PD2 // pin 1 (must be PD2 = INT0)
#define ROTENC_PIN2 _PD4 // pin 2

typedef enum ROTENC_DIR_e
{
    ROTENC_INC,
    ROTENC_DEC,
} ROTENC_DIR_t;

//static volatile uint32_t svRotEncLast;
//static volatile uint16_t svRotEncDt;

ISR(INT0_vect) // external interrupt 0
{
    osIsrEnter();

    const EVENT_t ev = PIN_GET(ROTENC_PIN2) ? EVENT_CNT_DEC : EVENT_CNT_INC;
    osQueueSend(&sEventQueue, &ev, -1);

//    const uint32_t msss = osTaskGetMsss();
//    svRotEncDt = msss - svRotEncLast;
//    svRotEncLast = msss;

    osIsrLeave();
}


/* ***** button ************************************************************** */

#define BUTTON_PIN  _PD3 // (must be PD3 = INT1)

#define BUTTON_PRESS_TIME 150

ISR(INT1_vect) // external interrupt 1
{
    osIsrEnter();

    static volatile uint32_t msss0;
    const uint32_t msss1 = osTaskGetTicks();
    if ( (msss1 - msss0) >= BUTTON_PRESS_TIME )
    {
        const EVENT_t ev = EVENT_SGL_PRESS;
        osQueueSend(&sEventQueue, &ev, -1);
    }
    msss0 = msss1;

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

    // rotary encoder
    PIN_INPUT(ROTENC_PIN1);            // signals
    PIN_PULLUP_ON(ROTENC_PIN1);
    PIN_INPUT(ROTENC_PIN2);
    PIN_PULLUP_ON(ROTENC_PIN2);
    SETBITS(EICRA, BIT(ISC01));        // falling-edge..
    CLRBITS(EICRA, BIT(ISC00));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT0));         // enable INT0
    SETBITS(EIFR, BIT(INTF0));         // clear INT0

    // button
    PIN_INPUT(BUTTON_PIN);
    PIN_PULLUP_ON(BUTTON_PIN);
    SETBITS(EICRA, BIT(ISC11));        // falling-edge..
    CLRBITS(EICRA, BIT(ISC10));        // ..triggers interrupt
    SETBITS(EIMSK, BIT(INT1));         // enable INT1
    SETBITS(EIFR, BIT(INTF1));         // clear INT1

    // event queue
    static EVENT_t sEventQueueBuf[EVENT_QUEUE_LEN];
    osQueueCreate(&sEventQueue, &sEventQueueBuf, EVENT_QUEUE_LEN, sizeof(EVENT_t));

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

    static int16_t count;

    while (ENDLESS)
    {
        osQueueDebug(&sEventQueue);
        EVENT_t ev;
        if (osQueueReceive(&sEventQueue, &ev, 1000))
        {
            switch (ev)
            {
                case EVENT_CNT_INC:
                    DEBUG("EVENT_CNT_INC");
                    count++;
                    break;
                case EVENT_CNT_DEC:
                    DEBUG("EVENT_CNT_DEC");
                    count--;
                    break;
                case EVENT_SGL_PRESS:
                    DEBUG("EVENT_SGL_PRESS");
                    break;
                case EVENT_DBL_PRESS:
                    DEBUG("EVENT_DBL_PRESS");
                    break;
                case EVENT_LNG_PRESS:
                    DEBUG("EVENT_LNG_PRESS");
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

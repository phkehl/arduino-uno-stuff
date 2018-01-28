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
#include "rotenc.h"        // ff: rotary encoder input

#include "ex12.h"


/* ***** application init **************************************************** */

// forward declarations
static void sAppStatus(char *str, const size_t size);
static void sAppTask(void *pArg);

// initialise the user application
void appInit(void)
{
    DEBUG("ex12: init");

    rotencInit();

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
    rotencClearEvents();

    static int16_t count;

    while (ENDLESS)
    {
        const ROTENC_EVENT_t ev = rotencGetEvent(1000);
        switch (ev)
        {
            case ROTENC_INC_DN:
                count += 9;
                FALLTHROUGH;
            case ROTENC_INC:
                count++;
                DEBUG("INC %"PRIu16, count);
                break;
            case ROTENC_DEC_DN:
                count -= 9;
                FALLTHROUGH;
            case ROTENC_DEC:
                count--;
                DEBUG("DEC %"PRIu16, count);
                break;
            case ROTENC_BTN:
                DEBUG("BTN");
                break;
            case ROTENC_BTN_LONG:
                DEBUG("LONG");
                break;
            case ROTENC_NONE:
                //DEBUG("NONE");
                break;
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

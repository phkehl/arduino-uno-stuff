/*!
    \file
    \brief flipflip's Arduino Uno stuff: flipflip's LED tester (see \ref PROJECTS_FFLEDTESTER)

    - Copyright (c) 2018 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFLEDTESTER

    Ardiono UNO Pins usage:
    - D0: \ref ROTENC pin 2
    - D1: \ref ROTENC pin 1 ("CLK")
    - D2: \ref ROTENC pin 3 ("SW")
    - D3:
    - D4:
    - D5:
    - D6:
    - D7:
    - D8:
    - D9:
    - D10:
    - D11:
    - D12:
    - D13:
    - A0:
    - A1:
    - A2:
    - A3:
    - A4:
    - A5:


    @{
*/

#include <string.h>        // libc: string operations

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
#include <avr/pgmspace.h>  // avr-libc: program Space Utilities
#include <avr/eeprom.h>    // avr-libc: EEPROM handling

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ws2801.h"        // ff: WS2801 LED driver
#include "ledfx.h"         // ff: LED effects
#include "rotenc.h"        // ff: rotary encoder input

#include "ffledtester.h"


/* ***** configuration ******************************************************* */


/* ***** application functions *********************************************** */


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
    /*const int n = */snprintf_P(str, size, PSTR("ffledtester..."));
}


/* ***** application init **************************************************** */

// initialise the user application
void appInit(void)
{
    DEBUG("ffledtester: init");

    rotencInit();
    ws2801Init();

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

//------------------------------------------------------------------------------

//@}
// eof

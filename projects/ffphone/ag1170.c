/*!
    \file
    \brief flipflip's Arduino Uno stuff: Silvertel Ag1170 Low Power Ringing SLIC module (see \ref PROJECTS_FFPHONE_AG1170)

    - Copyright (c) 2011-2017 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup PROJECTS_FFPHONE_AG1170
    @{
*/

#include <string.h>        // libc: string operations

#include <avr/io.h>        // avr: AVR device-specific IO definitions

#include "stdstuff.h"      // ff: useful macros and types
#include "arduinopins.h"   // ff: Arduino pins
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging output facility
#include "os.h"            // ff: operating system abstractions
#include "hw.h"            // ff: hardware abstraction
#include "sys.h"           // ff: system task

#include "ag1170.h"


/* *************************************************************************** */

#ifndef __DOXYGEN__ // STFU
static const char skStateStrs[][8] PROGMEM =
{
    { "UNKNOWN\0" }, { "READY\0" }, { "ERROR\0" }
};
#endif

AG1170_STATE_t sState;

__INLINE AG1170_STATE_t ag1170GetState(void)
{
    return sState;
}


/* ***** init **************************************************************** */

void ag1170Init(void)
{
    DEBUG("ag1170: init");

    //PIN_OUTPUT(AG1170_RESET_PIN);
    //PIN_HIGH(AG1170_RESET_PIN);

    sState = AG1170_STATE_UNKNOWN;

    // power-down FIXME: manual says "do not input logic 1, leave free running"
    PIN_OUTPUT(AG1170_PD_PIN);
    PIN_LOW(AG1170_PD_PIN); // FIXME: so this powers the module off?

    // ring mode
    PIN_OUTPUT(AG1170_RM_PIN);
    PIN_LOW(AG1170_RM_PIN);

    // forward/reverse
    PIN_OUTPUT(AG1170_FR_PIN);
    PIN_LOW(AG1170_FR_PIN);

    // switch hook
    PIN_INPUT(AG1170_SHK_PIN);
    PIN_PULLUP_ON(AG1170_SHK_PIN);

}


/* ***** task **************************************************************** */

static void sAg1170Task(void *pArg)
{
    UNUSED(pArg);

    // keep the AG1170 module going...
    while (ENDLESS)
    {
        {
            static AG1170_STATE_t sLastState;
            if (sLastState != sState)
            {
                PRINT("AG1170 %S -> %S", skStateStrs[sLastState], skStateStrs[sState]);
                sLastState = sState;
            }
        }
        osTaskDelay(1000);
    }
}

void ag1170Start(void)
{
    static uint8_t stack[250];
    static OS_TASK_t task;
    osTaskCreate("ag", 4, &task, stack, sizeof(stack), sAg1170Task, NULL);
}


/* ***** status ************************************************************** */

// make application status string
void ag1170Status(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("state=%S"), skStateStrs[sState]);
}


/* *************************************************************************** */
//@}
// eof

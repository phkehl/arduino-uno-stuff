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

AG1170_STATE_t sState;

__INLINE AG1170_STATE_t ag1170GetState(void)
{
    return sState;
}

const char *ag1170StateStr(const AG1170_STATE_t state)
{
    switch (state)
    {
        case AG1170_STATE_UNKNOWN : return PSTR("UNKNOWN");
        case AG1170_STATE_ERROR   : return PSTR("ERROR");
        case AG1170_STATE_ONHOOK  : return PSTR("ONHOOK");
        case AG1170_STATE_OFFHOOK : return PSTR("OFFHOOK");
        case AG1170_STATE_RINGING : return PSTR("RINGING");
    }
    return PSTR("???");
}

/* ***** init **************************************************************** */

void ag1170Init(void)
{
    DEBUG("ag1170: init");

    //PIN_OUTPUT(AG1170_RESET_PIN);
    //PIN_HIGH(AG1170_RESET_PIN);

    sState = AG1170_STATE_UNKNOWN;

    // power-down, connected via NPN transistor, i.e. Arduino GPIO high -> AG1170 power down
    PIN_OUTPUT(AG1170_PD_PIN);
    PIN_HIGH(AG1170_PD_PIN);

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

#define AG_POWER_ON()   PIN_LOW(AG1170_PD_PIN)
#define AG_POWER_OFF()  PIN_HIGH(AG1170_PD_PIN)
#define AG_IS_ONHOOK()  PIN_GET(AG1170_SHK_PIN) ? false : true
#define AG_IS_OFFHOOK() PIN_GET(AG1170_SHK_PIN) ? true : false

#define AG_RING_MODE_ON()  PIN_HIGH(AG1170_RM_PIN)
#define AG_RING_MODE_OFF() PIN_LOW(AG1170_RM_PIN); PIN_LOW(AG1170_FR_PIN)
#define AG_RING_FORWARD()  PIN_HIGH(AG1170_FR_PIN)
#define AG_RING_REVERSE()  PIN_LOW(AG1170_FR_PIN)


/* ***** ringing ************************************************************* */

#define AG_RING_FREQ       20
#define AG_RING_DURATION  750
#define AG_RING_PAUSE    1250

static uint8_t sRingState;
static uint8_t sRingCount;


bool ag1170Ring(const uint8_t count)
{
    if (sState != AG1170_STATE_ONHOOK)
    {
        ERROR("AG1170 cannot ring in %S state", ag1170StateStr(sState));
        return false;
    }

    AG_POWER_ON();
    sState = AG1170_STATE_RINGING;
    sRingState = 0;
    sRingCount = MAX(count, 1);

    return true;
}

void ag1170RingCancel(void)
{
    DEBUG("ag1170: ring cancel");
    CS_ENTER;
    sRingCount = 0;
    sRingState = 255;
    CS_LEAVE;
}


static bool sAgRing(void)
{
    const uint8_t delay  = 1000 / AG_RING_FREQ / 2;
    const uint8_t nRing  = AG_RING_DURATION / delay;
    const uint8_t nPause = AG_RING_PAUSE / delay;

#if ( ((AG_RING_DURATION + AG_RING_PAUSE) / (1000 / AG_RING_FREQ / 2)) > 250 )
#  error sRingState too small!
#endif

    if (sRingState == 0)
    {
        DEBUG("ag1170: ring %"PRIu8" %"PRIu8" %"PRIu8" %"PRIu8, sRingCount, delay, nRing, nPause);
        AG_RING_MODE_ON();
    }
    else if (sRingState < nRing)
    {
        if ((sRingState % 2) == 0)
        {
            AG_RING_FORWARD();
        }
        else
        {
            AG_RING_REVERSE();
        }
    }
    else if (sRingState == nRing)
    {
        AG_RING_MODE_OFF();
    }

    sRingState++;
    if (sRingState >= (nRing + nPause))
    {
        sRingState = 0;
        CS_ENTER;
        if (sRingCount)
        {
            sRingCount--;
        }
        CS_LEAVE;
    }

    if (sRingCount == 0)
    {
        DEBUG("ag1170: ring done");
        AG_RING_MODE_OFF();
        return false;
    }

    osTaskDelay(delay);
    return true;
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
                PRINT("AG1170 %S -> %S", ag1170StateStr(sLastState), ag1170StateStr(sState));
                sLastState = sState;
            }
        }

        switch (sState)
        {
            // unknown -> assume onhook
            case AG1170_STATE_UNKNOWN:
            {
                sState = AG1170_STATE_ONHOOK;
                break;
            }

            // error
            case AG1170_STATE_ERROR:
            {
                ERROR("AG1170 WTF?!");
                AG_POWER_OFF();
                osTaskDelay(2500);
                break;
            }

            // onhook: check for offhook condition
            case AG1170_STATE_ONHOOK:
            {
                AG_POWER_ON();
                osTaskDelay(100); // "the SLIC takes 50ms to power up"

                // power-down again and try later
                if (AG_IS_ONHOOK())
                {
                    AG_POWER_OFF();
                    osTaskDelay(1000);
                }
                // go to offhook state
                else
                {
                    sState = AG1170_STATE_OFFHOOK;
                }
                break;
            }

            // offhook: check for onhook condition
            case AG1170_STATE_OFFHOOK:
            {
                if (AG_IS_ONHOOK())
                {
                    AG_POWER_OFF();
                    sState = AG1170_STATE_ONHOOK;
                }
                else
                {
                    osTaskDelay(50);
                }
                break;
            }

            // ringing
            case AG1170_STATE_RINGING:
            {
                // check for off-hook condition
                if (AG_IS_OFFHOOK())
                {
                    AG_RING_MODE_OFF();
                    sState = AG1170_STATE_OFFHOOK;
                }
                // otherwise ring
                else
                {
                    if (!sAgRing())
                    {
                        sState = AG1170_STATE_UNKNOWN;
                    }
                }

                break;
            }
        }
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
    snprintf_P(str, size, PSTR("state=%S"), ag1170StateStr(sState));
}


/* *************************************************************************** */
//@}
// eof

/*!
    \file
    \brief flipflip's Arduino Uno stuff: system task (see \ref SYS)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup SYS
    @{
*/

//#include <stdio.h>         // libc: standard buffered input/output
//#include <stdarg.h>        // libc: variable argument lists
//#include <string.h>        // libc: string operations
//#include <math.h>          // libc: mathematical functions

//#include <avr/pgmspace.h>  // avr-libc: program space utilities

#include "stdstuff.h"      // ff: useful macros and types
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware
#include "sys.h"           // ff: system task

/* ************************************************************************** */

static uint8_t sSysTaskStack[FF_SYS_STACK_SIZE];

static void sSysTask(void *pArg);

void sysInit(void)
{
    DEBUG("sys: init (stack %"PRIu16", mon %"PRIu16")", (uint16_t)sizeof(sSysTaskStack), (uint16_t)FF_SYS_MON_PERIOD);
}

void sysCreateSystemTask(void)
{
    static OS_TASK_t task;
    osTaskCreate("sys", 3, &task, sSysTaskStack, sizeof(sSysTaskStack), sSysTask,  NULL);
}

//uint16_t sysGetInitStackPointer(void) __FORCEINLINE;
inline uint16_t sysGetInitStackPointer(void)
{
    // use the mon task's stack for the initialisation
    // leave the top 32 bytes alone, it's initialised by the task creation function with the context
    // and such also note that (modified) Atomthreads marks the bottom 50 bytes for later stack
    // checking so this leaves us with some bytes of stack we can use in the initialisation
    return (uint16_t)&sSysTaskStack[sizeof(sSysTaskStack) - 32];
}

/* ***** system task ******************************************************** */

#if (FF_SYS_MON_VERBOSE > 0)
SYS_MON_FUNC_t sSysMonFunc;

void sysRegisterMonFunc(SYS_MON_FUNC_t func)
{
    sSysMonFunc = func;
}
#else
void sysRegisterMonFunc(SYS_MON_FUNC_t func) { UNUSED(func); }
#endif


static void sSysTask(void *pArg)
{
    UNUSED(pArg);

    uint32_t msss = osTaskGetTicks();

    uint16_t tick = 0;

    while (ENDLESS)
    {
        osTaskDelayUntil(&msss, 10);
        tick++;

        /* ***** stuff to be done ten milliseconds ***** */

        hwTxFlush();


        /* ***** stuff to be done every second ***** */

        if ((tick % 100) == 0)
        {
            //DEBUG("sys.. %"PRIu16" %"PRIu32, tick, msss); hwTxFlush();

            hwAssertWatchdog();
        }


        /* ***** stuff to be done every FF_SYS_MON_PERIOD seconds ***** */

#if (FF_SYS_MON_VERBOSE > 0)
        if ((tick % (100 * FF_SYS_MON_PERIOD)) == 0)
        {
            //DEBUG("mon... %"PRIu16" %"PRIu32, tick, msss); hwTxFlush();
            tick = 0;

            // os status
            {
                char str[64];
                osStatus(str, sizeof(str));
                PRINT("mon: os: %s", str);
                hwTxFlush();
            }

            // task list
            osPrintTaskList();

            // hardware and application status
            {
                char str[64];
                hwStatus(str, sizeof(str));
                PRINT("mon: hw: %s", str);
                hwTxFlush();
                if (sSysMonFunc)
                {
                    sSysMonFunc(str, sizeof(str));
                    PRINT("mon: app: %s", str);
                    hwTxFlush();
                }
            }
        }
#endif // (FF_SYS_MON_VERBOSE > 0)
    }
}



/* ************************************************************************** */


//@}
// eof

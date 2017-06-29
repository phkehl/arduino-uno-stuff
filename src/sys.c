/*!
    \file
    \brief flipflip's Arduino Uno stuff: system task (see \ref SYS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup SYS
    @{
*/

#include "stdstuff.h"      // ff: useful macros and types
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware
#include "sys.h"           // ff: system task

/* ************************************************************************** */

static uint8_t sSysTaskStack[FF_SYS_TASK_STACK];

static void sSysTask(void *pArg);

void sysInit(void)
{
    DEBUG("sys: init (stack %"PRIu16", mon %"PRIu16")", (uint16_t)sizeof(sSysTaskStack), (uint16_t)FF_SYS_MON_PERIOD);
}

void sysCreateSystemTask(void)
{
    static OS_TASK_t task;
    osTaskCreate("sys", FF_SYS_TASK_PRIO, &task, sSysTaskStack, sizeof(sSysTaskStack), sSysTask, NULL);
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
    static uint8_t period;

    while (ENDLESS)
    {
        osTaskDelayUntil(&msss, 1000);

        /* ***** stuff to be done every second ***** */

        hwAssertWatchdog();
        period++;

        /* ***** stuff to be done every FF_SYS_MON_PERIOD seconds ***** */

#if (FF_SYS_MON_VERBOSE > 0)
        if ((period % FF_SYS_MON_PERIOD) == 0)
        {
            period = 0;

            // os status
            {
                char str[64];
                osStatus(str, sizeof(str));
                PRINT_W("mon: os: %s", str);
            }

            // task list
            osPrintTaskList();

            // hardware and application status
            {
                char str[FF_SYS_MON_STRLEN];
                hwStatus(str, sizeof(str));
                PRINT_W("mon: hw: %s", str);
                if (sSysMonFunc)
                {
                    sSysMonFunc(str, sizeof(str));
                    PRINT_W("mon: app: %s", str);
                }
            }
        }
#endif // (FF_SYS_MON_VERBOSE > 0)
    }
}



/* ************************************************************************** */


//@}
// eof

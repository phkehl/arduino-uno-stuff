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

#if ( (FF_SYS_SW_WATCHDOG < 0) || (FF_SYS_SW_WATCHDOG > 255) )
#  error Illegal value for FF_SYS_SW_WATCHDOG!
#endif

/* ************************************************************************** */

static uint8_t sSysTaskStack[FF_SYS_TASK_STACK];

static void sSysTask(void *pArg);

void sysInit(void)
{
    DEBUG("sys: init (stack %"PRIu16", mon %"PRIu16", prio %"PRIu8", wd %"PRIu8")",
        (uint16_t)sizeof(sSysTaskStack), (uint16_t)FF_SYS_MON_PERIOD,
        (uint8_t)FF_SYS_TASK_PRIO, (uint8_t)FF_SYS_SW_WATCHDOG);
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

#define SYS_TASK_PERIOD 1000

#if (FF_SYS_SW_WATCHDOG > 0)
static uint8_t sSysSwWatchdogCount;
void sysAssertSwWatchdog(void)
{
    sSysSwWatchdogCount = 0;
}
#endif

static void sSysTask(void *pArg)
{
    UNUSED(pArg);

    uint32_t msss = osTaskGetTicks();
    static uint8_t period;

    while (ENDLESS)
    {
        osTaskDelayUntil(&msss, SYS_TASK_PERIOD);

        /* ***** stuff to be done every second ***** */

        hwAssertWatchdog();
        period++;

#if (FF_SYS_SW_WATCHDOG > 0)
        sSysSwWatchdogCount++;
        if (sSysSwWatchdogCount >= FF_SYS_SW_WATCHDOG)
        {
            ERROR("sys: sw watchdog");
            osTaskDelay(100);
            hwPanic(HW_PANIC_SYS, 0x01, 0x00);
        }
#endif

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
            {
                const uint16_t dt = (SYS_TASK_PERIOD * FF_SYS_MON_PERIOD) + (msss % SYS_TASK_PERIOD);
                osPrintTaskList(dt);
            }

            // hardware and application status
            {
                char str[FF_SYS_MON_STRLEN];
                hwStatus(str, sizeof(str));
                PRINT_W("mon: hw: %s", str);
                if (sSysMonFunc)
                {
                    str[0] = '\0';
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

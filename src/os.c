/*!
    \file
    \brief flipflip's Arduino Uno stuff: operating system abstraction (see \ref OS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \addtogroup OS
    @{

    \todo implement precise task runtime statistics, incl. IRQ load statistics (if that's even possible...)

    \todo look into http://mark3os.com/docs/html/index.html
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdlib.h>        // libc: standard library
#include <string.h>        // libc: string operations

#include <atomthreads.h>   // Atomthreads: kernel

#include <avr/interrupt.h> // avr: interrupt things
#include <avr/pgmspace.h>  // avr: program space utilities
#include <util/atomic.h>   // avr: atomically and non-atomically executed code blocks

#include "stdstuff.h"      // ff: useful macros and types
#include "config.h"        // ff: configuration
#include "debug.h"         // ff: debugging
#include "os.h"            // ff: operating system
#include "hw.h"            // ff: hardware


#if (F_CPU != 16000000)
#  warning F_CPU not 16MHz. System tick will not run at 1 Hz!
#endif


#if (FF_OS_HEAP_SIZE > 0)
static char sOsHeapMem[FF_OS_HEAP_SIZE]; // heap memory
#endif

enum
{
    OS_PANIC_INIT       = 0x11,

    OS_PANIC_TASK       = 0x21,

    OS_PANIC_SEM_CREATE = 0x31,
    OS_PANIC_SEM_TAKE   = 0x32,
    OS_PANIC_SEM_GIVE   = 0x33,
    OS_PANIC_SEM_DELETE = 0x34,

    OS_PANIC_MX_CREATE  = 0x41,
    OS_PANIC_MX_CLAIM   = 0x42,
    OS_PANIC_MX_RELEASE = 0x43,
    OS_PANIC_MX_DELETE  = 0x44,

    OS_PANIC_Q_CREATE   = 0x51,
    OS_PANIC_Q_SEND     = 0x52,
    OS_PANIC_Q_RECEIVE  = 0x53,
    OS_PANIC_Q_DELETE   = 0x54,

    OS_PANIC_TIMER_ARM  = 0x61,
    OS_PANIC_TIMER_KILL = 0x64,
};


/* ***** os initialisation ************************************************** */

static ATOM_TCB *spLastCreatedTask = NULL;

// idle thread stack
static uint8_t sOsIdleTaskStack[FF_OS_IDLE_STACK];

void osInit(void)
{

#if (FF_OS_HEAP_SIZE > 0)
    __malloc_heap_start = (void *)sOsHeapMem;
    __malloc_heap_end   = (void *)(sOsHeapMem + sizeof(sOsHeapMem));
#else
    __malloc_heap_start = (void *)RAMEND;
    __malloc_heap_end   = (void *)RAMEND;
#endif

    DEBUG("os: init (heap %"PRIu16"@%p, idl stack %"PRIu16"@%p, prio %"PRIu8")",
        (uint16_t)FF_OS_HEAP_SIZE, __malloc_heap_start, (uint16_t)sizeof(sOsIdleTaskStack),
        sOsIdleTaskStack, (uint8_t)IDLE_THREAD_PRIORITY);

    const uint8_t res = atomOSInit(sOsIdleTaskStack, sizeof(sOsIdleTaskStack), true);
    if (res == ATOM_OK)
    {
        // setup and enable the system tick timer (TCCR = Timer/Counter Control Register)
        TCCR0A = /*BIT(COM0A0)              // toggle OC0A on compare match
               |*/ BIT(WGM01);              // clear timer on compare (CTC) mode TOP=OC0A
        // 1000 Hz
        // 16_000_000 / 64 = 250000 clk/step
        // "1 TC step" = 0.004ms, 250 steps = 1.000ms
        TCCR0B = BIT(CS00) | BIT(CS01);    // prescale clk/64
        OCR0A  = (F_CPU / 64 / 1000) - 1;  // 1ms ticks on output compare register A (= 249.0 @ 16MHz)

        // 100 Hz
        //TCCR0B = BIT(CS00) | BIT(CS02);  // prescale clk/1024
        //OCR0A  = (F_CPU / 1024 / 100);   // 10ms ticks on output compare register A (= 156.25 @ 16MHz)

        //SETBITS(DDRD, BIT(PD6));         // configure OC0A pin as output
        TCNT0  = 0;                      // timer/counter0 register
        TIFR0  = 0;                      // timer/counter0 interrupt flag register
        TIMSK0 = BIT(OCIE0A);            // timer/counter0 output compare match A interrupt enable

        spLastCreatedTask = atomGetIdleTCB();
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_INIT, res);
    }
}


// task tick routine, runs the scheduler
//ISR(TIMER0_COMPA_vect, ISR_NAKED)
ISR(TIMER0_COMPA_vect)
{
    // trigger systicks debug signal (LED)
    hwLedTickToggle();
    // note that the load LED is toggled in atomThreadSwitch()

    // add runtime to task
    // FIXME: other stuff is happening, too (timers, interrupts)
    ATOM_TCB *pTCB = atomCurrentContext();
    if (pTCB != NULL)
    {
        pTCB->runtime++;
    }

    atomIntEnter();     // call the interrupt entry routine
    atomTimerTick();    // call the timer handler
    atomIntExit(true);  // call the interrupt exit routine (and schedule in pending tasks)
}


//static uint8_t sOsIsrStack[150];
//static int16_t sOsPreIsrSP;

__FORCEINLINE void osIsrEnter(void)
{
    // sOsPreIsrSP = SP;
    // SP = (int16_t)&sOsIsrStack[sizeof(sOsIsrStack)-1];
    atomIntEnter();
}


__FORCEINLINE void osIsrLeave(void)
{
    atomIntExit(false);
    // SP = sOsPreIsrSP;
}

static uint8_t sOsCriticalNesting = 0;
static uint8_t sOsCriticalState = 0;

inline void osCsEnter(void)
{
    if (!sOsCriticalNesting)
    {
        sOsCriticalState = SREG;
        cli();
    }
    sOsCriticalNesting++;
}


inline void osCsLeave(void)
{
    // FIXME: hwPanic() if sOsCriticalNesting == 0
    sOsCriticalNesting--;
    if (sOsCriticalNesting == 0)
    {
        //sei();
        SREG = sOsCriticalState;
    }
}


#if (FF_OS_HEAP_SIZE > 0)

// from avr-libc-1.8.1/libc/stdlib/malloc.c
extern void *__brkval;
struct __freelist { size_t sz; struct __freelist *nx; };
extern struct __freelist *__flp;


void osStatus(char *str, const size_t size)
{
    // memory never allocated so far
    const uint16_t unalloc = __brkval ? (char *)__malloc_heap_end - (char *)__brkval : FF_OS_HEAP_SIZE;
    //DEBUG("unalloc %"PRIu16" %p %p", unalloc, __malloc_heap_end, __brkval);

    uint16_t totFree = unalloc; // total free heap memory
    uint16_t minFree = unalloc; // biggest free chunk

    // go through chunks in malloc()'s free list
    struct __freelist *current;
    for (current = __flp; current; current = current->nx)
    {
        totFree += 2; // memory block header
        totFree += current->sz;
        //DEBUG("freelist %"PRIu16" %p", (uint16_t)current->sz, current);
        if (current->sz > minFree)
        {
            minFree = current->sz;
        }
    }

    snprintf_P(str, size, PSTR("ticks=%"PRIu32" heap=%"PRIu16"/%"PRIu16"/%"PRIu16),
        osTaskGetTicks(), minFree, totFree,
        (uint16_t)FF_OS_HEAP_SIZE/*(uint16_t)(__malloc_heap_end - __malloc_heap_start)*/);
}

#else

void osStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("ticks=%"PRIu32), osTaskGetTicks());
}

#endif // (FF_OS_HEAP_SIZE > 0)


void osPrintTaskList(void)
{
    uint8_t num = 0;

    // count number of tasks
    CS_ENTER;

    ATOM_TCB *pTask = atomGetIdleTCB();
    while (pTask)
    {
        pTask = pTask->next;
        num++;
    }

    CS_LEAVE;

    ATOM_TCB *tasks[num];
    uint16_t rtTot = 0;
    uint16_t rtAll[num];
    //uint16_t pc[num];

    // sum up total runtime and create list of TCBs
    CS_ENTER;

    ATOM_TCB *pTask = atomGetIdleTCB();
    for (uint8_t ix = 0; ix < num; ix++)
    {
        rtTot += pTask->runtime;
        rtAll[ix] = pTask->runtime;
        pTask->runtime = 0;
        tasks[ix] = pTask;
        pTask = pTask->next;
        //pc[ix] = *( (uint16_t *)((uint8_t *)pTask->stack_bottom - sizeof(uint16_t)) );
    }

    CS_LEAVE;

    for (uint8_t ix = 0; ix < num; ix++)
    {
        const OS_TASK_t *pkTask = tasks[ix];
#  ifdef ATOM_STACK_CHECKING
        uint16_t used, free;
        atomThreadStackCheck(pkTask, &used, &free);
#  else
        uint32_t free = 0;
#  endif
        const uint16_t load = (uint16_t)(((float)rtAll[ix] * 100.0f / (float)rtTot * 10.0f) + 0.5);
        const uint8_t loadInt = load / 10;
        const uint8_t loadFrac = load - (10 * loadInt);
        PRINT_W("mon: tsk: %"PRIu8" %-3s %c %"PRIu8" %2"PRIu16" %2"PRIu8".%"PRIu8,// " PC %p",
            ix, pkTask->name,
            pkTask->suspended ? 'S' : 'R', pkTask->priority, free, loadInt, loadFrac);//, pc[ix]);
    }
}


/* ***** scheduler functions ************************************************ */

typedef void (*ATOM_TASKFUNC_t)(uint32_t);

void osTaskCreate(const char *taskName, const uint16_t priority, OS_TASK_t *pTask,
    uint8_t *pStack, const uint16_t stackSize, OS_TASKFUNC_t taskFunc, void *taskParam)
{
    if ( (priority >= IDLE_THREAD_PRIORITY) ||
        (atomThreadCreate((ATOM_TCB *)pTask, (uint8_t)priority, (ATOM_TASKFUNC_t)taskFunc,
            (uint16_t)(void *)taskParam, pStack, stackSize, true) != ATOM_OK))
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_TASK, (((uint32_t)taskName[0]) << 16) | (((uint32_t)taskName[1]) << 8) | ((uint32_t)taskName[2]));
    }
    else
    {
        DEBUG("os: task '%s' (stack %"PRIu16"@%p, prio %"PRIu16", tcb %p)",
              taskName, stackSize, pStack, priority, pTask);

        pTask->name = taskName;

        spLastCreatedTask->next = pTask;
        pTask->next = NULL;
        spLastCreatedTask = pTask;
    }
}


static bool sOsTaskSchedulerRunning = false;


void osTaskStartScheduler(void)
{
    DEBUG("os: start");
    sOsTaskSchedulerRunning = true;
    // reuse part of the idle thread's stack for the stack required
    // during this startup function.
    //SP = (int)sTasksIdleStack[sizeof(sTasksIdleStack) - 32];
    //SP = RAMEND;
    atomTimeSet(-1);
    atomOSStart();
}

void osTaskSuspendScheduler(void)
{
    CLRBITS(TIMSK0, OCIE0A);
    sOsTaskSchedulerRunning = false;
}


void osTaskResumeScheduler(void)
{
    sOsTaskSchedulerRunning = true;
    SETBITS(TIMSK0, OCIE0A);
}


bool osTaskIsSchedulerRunning(void)
{
    return sOsTaskSchedulerRunning;
}


void osTaskDelay(const uint32_t timeout)
{
    atomTimerDelay(timeout);
}


inline bool osTaskDelayUntil(uint32_t *pPrevTick, const uint32_t incrTicks)
{
    const uint32_t targetTick = *pPrevTick + incrTicks;
    const uint32_t tickNow = osTaskGetTicks();
    *pPrevTick = targetTick;
    if (tickNow < targetTick)
    {
        osTaskDelay(targetTick - tickNow);
        return true;
    }
    else
    {
        return false;
    }
}


inline uint32_t osTaskGetTicks(void)
{
    return atomTimeGet();
}

inline uint32_t osTaskGetMsss(void)
{
    return atomTimeGet();
}


/* ***** semaphores ********************************************************* */

void osSemaphoreCreate(OS_SEMAPHORE_t *pSem, const uint16_t iniCount)
{
    const uint8_t res = atomSemCreate((ATOM_SEM *)pSem, iniCount);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_CREATE, res);
    }
}

bool osSemaphoreTake(OS_SEMAPHORE_t *pSem, const int32_t timeout)
{
    uint16_t res = atomSemGet((ATOM_SEM *)pSem, timeout);
    if (res == ATOM_OK)
    {
        return true;
    }
    else if ( (res == ATOM_WOULDBLOCK) || (res == ATOM_TIMEOUT) )
    {
        return false;
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_TAKE, res);
    }
    // we never end up here
    return false;
}

void osSemaphoreGive(OS_SEMAPHORE_t *pSem, const bool binary)
{
    if (binary)
    {
        atomSemResetCount((ATOM_SEM *)pSem, 0);
    }
    const uint8_t res = atomSemPut((ATOM_SEM *)pSem);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_GIVE, res);
    }
}

void osSemaphoreDelete(OS_SEMAPHORE_t *pSem)
{
    const uint8_t res = atomSemDelete((ATOM_SEM *)pSem);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_DELETE, res);
    }
}



/* ***** mutex functions **************************************************** */

void osMutexCreate(OS_MUTEX_t *pMutex)
{
    const uint8_t res = atomMutexCreate((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_CREATE, res);
    }
}


bool osMutexClaim(OS_MUTEX_t *pMutex, const int32_t timeout)
{

    const uint16_t res = atomMutexGet((ATOM_MUTEX *)pMutex, (int16_t)timeout);
    if (res == ATOM_OK)
    {
        return true;
    }
    else if ( (res == ATOM_WOULDBLOCK) || (res == ATOM_TIMEOUT) )
    {
        return false;
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_CLAIM, res);
    }
    // we never end up here
    return false;
}


void osMutexRelease(OS_MUTEX_t *pMutex)
{
    const uint8_t res = atomMutexPut((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_RELEASE, res);
    }
}


void osMutexDelete(OS_MUTEX_t *pMutex)
{
    const uint8_t res = atomMutexDelete((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_DELETE, res);
    }
}



/* ***** queue functions **************************************************** */

void osQueueCreate(OS_QUEUE_t *pQueue, void *pBuf, const uint16_t length, const uint16_t itemSize)
{
    const uint8_t res = atomQueueCreate((ATOM_QUEUE *)pQueue, pBuf, length, itemSize);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_CREATE, res);
    }
}

bool osQueueSend(OS_QUEUE_t *pQueue, const void *pkMsg, const int32_t timeout)
{
    const uint8_t res = atomQueuePut((ATOM_QUEUE *)pQueue, timeout, pkMsg);
    if (res == ATOM_OK)
    {
        return true;
    }
    else if ( (res == ATOM_WOULDBLOCK) || (res == ATOM_TIMEOUT) )
    {
        return false;
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_SEND, res);
    }
    // we never end up here
    return false;
}


bool osQueueReceive(OS_QUEUE_t *pQueue, const int32_t timeout, void *pMsg)
{
    const uint8_t res = atomQueueGet((ATOM_QUEUE *)pQueue, timeout, pMsg);
    if (res == ATOM_OK)
    {
        return true;
    }
    else if ( (res == ATOM_WOULDBLOCK) || (res == ATOM_TIMEOUT) )
    {
        return false;
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_RECEIVE, res);
    }
    // we never end up here
    return false;
}


void osQueueDelete(OS_QUEUE_t *pQueue)
{
    const uint8_t res = atomQueueDelete((ATOM_QUEUE *)pQueue);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_DELETE, res);
    }
}



/* ***** timer functions **************************************************** */

void osTimerArm(OS_TIMER_t *pTimer, OS_TIMERFUNC_t cb, void *pArg, uint32_t timeout, uint16_t repeat)
{
    pTimer->cb_func   = cb;
    pTimer->cb_data   = pArg;
    pTimer->cb_ticks  = timeout;
    pTimer->cb_repeat = repeat;
    const uint8_t res = atomTimerRegister((ATOM_TIMER *)pTimer);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_TIMER_ARM, res);
    }
}

bool osTimerKill(OS_TIMER_t *pTimer)
{
    const uint8_t res = atomTimerCancel((ATOM_TIMER *)pTimer);
    if (res == ATOM_OK)
    {
        return true;
    }
    else if (res == ATOM_ERR_NOT_FOUND)
    {
        return false;
    }
    else
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_TIMER_KILL, res);
    }
    // we never end up here
    return false;
}


/* ************************************************************************** */

//@}
// eof

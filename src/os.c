/*!
    \file
    \brief flipflip's Arduino Uno stuff: operating system abstraction (see \ref OS)

    - Copyright (c) 2016 Philippe Kehl <flipflip at oinkzwurgl dot org>

    \addtogroup OS
    @{
*/

#include <stdio.h>         // libc: standard buffered input/output
#include <stdlib.h>        // libc: standard library
#include <string.h>        // libc: string operations

#include <atomthreads.h>   // Atomthreads: kernel

#include <avr/interrupt.h> // avr: global manipulation of the interrupt flag
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
static CH sOsHeapMem[FF_OS_HEAP_SIZE]; // heap memory
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
static U1 sOsIdleTaskStack[FF_OS_IDLE_STACK_SIZE];

void osInit(void)
{

#if (FF_OS_HEAP_SIZE > 0)
    __malloc_heap_start = (void *)sOsHeapMem;
    __malloc_heap_end   = (void *)(sOsHeapMem + sizeof(sOsHeapMem));
#else
    __malloc_heap_start = (void *)RAMEND;
    __malloc_heap_end   = (void *)RAMEND;
#endif

    DEBUG("os: init (heap %"F_U"@%p, idl stack %"F_U"@%p, prio %"F_U1")",
        (U)FF_OS_HEAP_SIZE, __malloc_heap_start, (U)sizeof(sOsIdleTaskStack),
        sOsIdleTaskStack, (U1)IDLE_THREAD_PRIORITY);

    const U1 res = atomOSInit(sOsIdleTaskStack, sizeof(sOsIdleTaskStack), true);
    if (res == ATOM_OK)
    {
        // setup and enable the system tick timer (TCCR = Timer/Counter Control Register)
        TCCR0A = /*BIT(COM0A0)             // toggle OC0A on compare match
               |*/ BIT(WGM01);              // clear timer on compare (CTC) mode TOP=OC0A
        // 1000 Hz
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


//static volatile U2 svOsRuntimeCounter = 0;

// task tick routine, runs the scheduler
//ISR(TIMER0_COMPA_vect, ISR_NAKED)
ISR(TIMER0_COMPA_vect)
{
    // trigger systicks debug signal (LED)
    hwLedTickToggle();
    // note that the load LED is toggled in atomThreadSwitch()

    // sum up runtime per task
    ATOM_TCB *pTCB = atomCurrentContext();
    if (pTCB != NULL)
    {
        //const U4 rtcnt = hwGetRuntimeCounter();
        //const U2 delta = (rtcnt - svOsRuntimeCounter);
        //pTCB->runtime += delta;
        //svOsRuntimeCounter = rtcnt;

        // FIXME this is only an approximation
        pTCB->runtime++;
    }

    atomIntEnter();     // call the interrupt entry routine
    atomTimerTick();    // call the timer handler
    atomIntExit(true);  // call the interrupt exit routine (and schedule in pending tasks)
}


//static U1 sOsIsrStack[150];
//static I sOsPreIsrSP;

inline void osIsrEnter(void)
{
    // sOsPreIsrSP = SP;
    // SP = (I)&sOsIsrStack[sizeof(sOsIsrStack)-1];
    atomIntEnter();
}


inline void osIsrLeave(void)
{
    atomIntExit(false);
    // SP = sOsPreIsrSP;
}

static U1 sOsCriticalNesting = 0;
static U1 sOsCriticalState = 0;

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
    const U unalloc = __brkval ? (char *)__malloc_heap_end - (char *)__brkval : FF_OS_HEAP_SIZE;
    //DEBUG("unalloc %"F_U" %p %p", unalloc, __malloc_heap_end, __brkval);

    U totFree = unalloc; // total free heap memory
    U minFree = unalloc; // biggest free chunk

    // go through chunks in malloc()'s free list
    struct __freelist *current;
    for (current = __flp; current; current = current->nx)
    {
        totFree += 2; // memory block header
        totFree += current->sz;
        //DEBUG("freelist %"F_U" %p", (U)current->sz, current);
        if (current->sz > minFree)
        {
            minFree = current->sz;
        }
    }

    snprintf_P(str, size, PSTR("msss=%"F_U4" heap=%"F_U"/%"F_U"/%"F_U),
        osTaskGetTicks(), minFree, totFree,
        (U)FF_OS_HEAP_SIZE/*(U)(__malloc_heap_end - __malloc_heap_start)*/);
}

#else

void osStatus(char *str, const size_t size)
{
    snprintf_P(str, size, PSTR("msss=%"F_U4), osTaskGetTicks());
}

#endif // (FF_OS_HEAP_SIZE > 0)


void osPrintTaskList(void)
{
    U1 num = 0;

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
    U2 rtTot = 0;
    U2 rtAll[num];
    //U2 pc[num];

    // sum up total runtime and create list of TCBs
    CS_ENTER;

    ATOM_TCB *pTask = atomGetIdleTCB();
    for (U1 ix = 0; ix < num; ix++)
    {
        rtTot += pTask->runtime;
        rtAll[ix] = pTask->runtime;
        pTask->runtime = 0;
        tasks[ix] = pTask;
        pTask = pTask->next;
        //pc[ix] = *( (U2 *)((U1 *)pTask->stack_bottom - sizeof(U2)) );
    }

    CS_LEAVE;

    for (U1 ix = 0; ix < num; ix++)
    {
        const OS_TASK_t *pkTask = tasks[ix];
#  ifdef ATOM_STACK_CHECKING
        U4 used, free;
        atomThreadStackCheck(pkTask, &used, &free);
#  else
        U4 free = 0;
#  endif
        const U2 load = (U2)(((R4)rtAll[ix] * 100.0f / (R4)rtTot * 10.0f) + 0.5);
        const U1 loadInt = load / 10;
        const U1 loadFrac = load - (10 * loadInt);
        PRINT("mon: tsk: %"F_U1" %s %c %"F_U1" %2"F_U4" %2"F_U1".%"F_U1,// " PC %p",
            ix, pkTask->name,
            pkTask->suspended ? 'S' : 'R', pkTask->priority, free, loadInt, loadFrac);//, pc[ix]);
        hwTxFlush();
    }
}


/* ***** scheduler functions ************************************************ */

typedef void (*ATOM_TASKFUNC_t)(U4);

void osTaskCreate(const CH *taskName, const U priority, OS_TASK_t *pTask,
    U1 *pStack, const U stackSize, OS_TASKFUNC_t taskFunc, void *taskParam)
{
    if ( (priority >= IDLE_THREAD_PRIORITY) ||
        (atomThreadCreate((ATOM_TCB *)pTask, (U1)priority, (ATOM_TASKFUNC_t)taskFunc,
            (U2)(void *)taskParam, pStack, stackSize, true) != ATOM_OK))
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_TASK, (((U4)taskName[0]) << 16) | (((U4)taskName[1]) << 8) | ((U4)taskName[2]));
    }
    else
    {
        DEBUG("os: task '%s' (stack %" F_U "@%p, prio %" F_U", tcb %p)",
              taskName, stackSize, pStack, priority, pTask);

        pTask->name = taskName;

        spLastCreatedTask->next = pTask;
        pTask->next = NULL;
        spLastCreatedTask = pTask;
    }
}


static L sOsTaskSchedulerRunning = false;


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


L osTaskIsSchedulerRunning(void)
{
    return sOsTaskSchedulerRunning;
}


void osTaskDelay(const U4 timeout)
{
    atomTimerDelay(timeout);
}


inline void osTaskDelayUntil(U4 *pPrevTick, const U4 incrTicks)
{
    const U4 targetTick = *pPrevTick + incrTicks;
    const U4 tickNow = osTaskGetTicks();
    if (tickNow < targetTick)
    {
        osTaskDelay(targetTick - tickNow);
    }
    *pPrevTick = targetTick;
}


inline U4 osTaskGetTicks(void)
{
    return atomTimeGet();
}


/* ***** semaphores ********************************************************* */

void osSemaphoreCreate(OS_SEMAPHORE_t *pSem, const U iniCount)
{
    const U1 res = atomSemCreate((ATOM_SEM *)pSem, iniCount);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_CREATE, res);
    }
}

L osSemaphoreTake(OS_SEMAPHORE_t *pSem, const I4 timeout)
{
    U res = atomSemGet((ATOM_SEM *)pSem, (I)timeout);
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

void osSemaphoreGive(OS_SEMAPHORE_t *pSem)
{
    const U1 res = atomSemPut((ATOM_SEM *)pSem);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_GIVE, res);
    }
}

void osSemaphoreDelete(OS_SEMAPHORE_t *pSem)
{
    const U1 res = atomSemDelete((ATOM_SEM *)pSem);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_SEM_DELETE, res);
    }
}



/* ***** mutex functions **************************************************** */

void osMutexCreate(OS_MUTEX_t *pMutex)
{
    const U1 res = atomMutexCreate((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_CREATE, res);
    }
}


L osMutexClaim(OS_MUTEX_t *pMutex, const I4 timeout)
{

    const U res = atomMutexGet((ATOM_MUTEX *)pMutex, (I)timeout);
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
    const U1 res = atomMutexPut((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_RELEASE, res);
    }
}


void osMutexDelete(OS_MUTEX_t *pMutex)
{
    const U1 res = atomMutexDelete((ATOM_MUTEX *)pMutex);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_MX_DELETE, res);
    }
}



/* ***** queue functions **************************************************** */

void osQueueCreate(OS_QUEUE_t *pQueue, void *pBuf, const U length, const U itemSize)
{
    const U1 res = atomQueueCreate((ATOM_QUEUE *)pQueue, pBuf, length, itemSize);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_CREATE, res);
    }
}

L osQueueSend(OS_QUEUE_t *pQueue, const void *pkMsg, const I4 timeout)
{
    const U1 res = atomQueuePut((ATOM_QUEUE *)pQueue, timeout, pkMsg);
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


L osQueueReceive(OS_QUEUE_t *pQueue, void *pItem, const I4 timeout, void *pMsg)
{
    const U1 res = atomQueueGet((ATOM_QUEUE *)pQueue, timeout, pMsg);
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
    const U1 res = atomQueueDelete((ATOM_QUEUE *)pQueue);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_Q_DELETE, res);
    }
}



/* ***** timer functions **************************************************** */

void osTimerArm(OS_TIMER_t *pTimer, OS_TIMERFUNC_t cb, void *pArg, U4 timeout, U2 repeat)
{
    pTimer->cb_func   = cb;
    pTimer->cb_data   = pArg;
    pTimer->cb_ticks  = timeout;
    pTimer->cb_repeat = repeat;
    const U1 res = atomTimerRegister((ATOM_TIMER *)pTimer);
    if (res != ATOM_OK)
    {
        hwPanic(HW_PANIC_OS, OS_PANIC_TIMER_ARM, res);
    }
}

L osTimerKill(OS_TIMER_t *pTimer)
{
    const U1 res = atomTimerCancel((ATOM_TIMER *)pTimer);
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

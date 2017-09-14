/*!
    \file
    \brief flipflip's Arduino Uno stuff: operating system abstraction (see \ref OS)

    - Copyright (c) 2016 Philippe Kehl (flipflip at oinkzwurgl dot org)

    \defgroup OS Operating System
    \ingroup FF

    This implements the operating system abstraction.

    Note that some functions may panic via hwPanic() on unexpected errors.

    Configuration:
    - #FF_OS_IDLE_STACK
    - #FF_OS_HEAP_SIZE

    @{
*/

#ifndef __OS_H__
#define __OS_H__

#include "stdstuff.h"      // ff: useful macros and types
#include "atomtypes.h"     // Atomthreads: types

/* ************************************************************************** */
// Operating System (OS) Initialisation and Utils (internal)

// initialise operating system stuff
void osInit(void);

// operation system status for the system monitor
void osStatus(char *str, const size_t size);
void osPrintTaskList(void);



/* ************************************************************************** */
/*!
    \name Scheduler Functions
    @{
*/

//! task control block
typedef ATOM_TCB OS_TASK_t;

//! task function type
typedef void (* OS_TASKFUNC_t)(void *);

//! create a task
/*!
    Creates task.

    \param[in] taskName   the name of the task (string of length 3)
    \param[in] priority   task priority (< #IDLE_THREAD_PRIORITY)
    \param[in] pTask      pointer to the task control block
    \param[in] pStack     pointer to the stack
    \param[in] stackSize  size of \c *pStack
    \param[in] taskFunc   task function
    \param[in] taskParam  task parameter, passed-through to the \c taskFunc
*/
void osTaskCreate(const char *taskName, const uint16_t priority, OS_TASK_t *pTask,
    uint8_t *pStack, const uint16_t stackSize, OS_TASKFUNC_t taskFunc, void *taskParam);

//! start the scheduler, i.e. the tasks
void osTaskStartScheduler(void);

//! suspend scheduler
void osTaskSuspendScheduler(void);

//! resume scheduler
void osTaskResumeScheduler(void);

//! check if scheduler is running
/*!
    \returns \c true if the scheduler is running, \c false otherwise
*/
bool osTaskIsSchedulerRunning(void);

//! get ticks since start
uint32_t osTaskGetTicks(void);

//! get milliseconds since start
uint32_t osTaskGetMsss(void);


//! delay task
/*!
    \param[in] timeout  number of ticks to wait
*/
void osTaskDelay(const uint32_t timeout);

//! delay task until specific time
/*!
    \param[in,out] pPrevTick  time reference on input, set to target wake time on output
    \param[in]     incrTicks  number of ticks to wait relative to \c pPrevTick
    \returns true if the task was delayed, false if it was too late for that
*/
bool osTaskDelayUntil(uint32_t *pPrevTick, const uint32_t incrTicks);

//! interrupt enter routine, call first in all ISRs
void osIsrEnter(void);

//! interrupt leave routine, call last in all ISRs
void osIsrLeave(void);

//@}

/* ************************************************************************** */
/*!
    \name Critical Sections
    @{
*/

//! enter a critical section, asserting that interrupts are off \hideinitializer
#define CS_ENTER do { osCsEnter();

//! leave a critical section, re-enabling the interrupts if necessary \hideinitializer
#define CS_LEAVE      osCsLeave(); } while (0)

//@}

void osCsEnter(void);
void osCsLeave(void);


/* ************************************************************************** */
/*!
    \name Semaphore Functions

    See also \ref ATOM_SEM.

    @{
*/

//! a semaphore
typedef ATOM_SEM OS_SEMAPHORE_t;

//! initialise semaphore
/*!
    \param[in,out] pSem      the semaphore
    \param[in]     iniCount  initial count
*/
void osSemaphoreCreate(OS_SEMAPHORE_t *pSem, const uint16_t iniCount);

//! take semaphore (decrement count)
/*!
    Decrements the current count value and returns. If the current count value is zero the call will
    block until the count is incremented by another task, the semaphore is deleted or a timeout
    occured.

    \param[in,out] pSem     the semaphore
    \param[in]     timeout  maximum time to wait (0 for endless) or -1 to check

    \returns \c true if the semaphore was taken (decremented),
             \c false on timeout or if the call would block with \c timeout >= 0
*/
bool osSemaphoreTake(OS_SEMAPHORE_t *pSem, const int32_t timeout);

//! give semaphore (increment count)
/*!
    This will wake up threads waiting on the semaphore.

    \param[in,out] pSem    the semaphore
    \param[in]     binary  set to true if this a binary semaphore (counting only up to 1)
*/
void osSemaphoreGive(OS_SEMAPHORE_t *pSem, const bool binary);

//! delete semaphore
/*!
    \param[in,out] pSem  the semaphore
*/
void osSemaphoreDelete(OS_SEMAPHORE_t *pSem);

//@}


/* ************************************************************************** */
/*!
    \name Mutex Functions

    See also \ref ATOM_MUTEX.

    @{
*/

//! a mutex
typedef ATOM_MUTEX OS_MUTEX_t;

//! initialise mutex
/*!
    \param[in,out] pMutex  the mutex
*/
void osMutexCreate(OS_MUTEX_t *pMutex);

//! claim mutex (take lock)
/*!
    Takes ownership of the mutex. If the mutex is owned by another thread it blocks until the mutex
    has been given back by that thread or the timeout occurred.

    \param[in,out] pMutex     the mutex
    \param[in]     timeout  maximum time to wait (0 for endless) or -1 to check

    \returns \c true if the mutex was available,
             \c false on timeout or if the call would block with \c timeout >= 0
*/
bool osMutexClaim(OS_MUTEX_t *pMutex, const int32_t timeout);

//! release mutex (give lock)
/*!
    \param[in,out] pMutex     the mutex
*/
void osMutexRelease(OS_MUTEX_t *pMutex);


//! delete mutex
/*!
    \param[in,out] pMutex     the mutex
*/
void osMutexDelete(OS_MUTEX_t *pMutex);

//@}

/* ************************************************************************** */
/*!
    \name Timer Functions

    See also \ref ATOM_TIMER.

    @{
*/

//! a timer
typedef ATOM_TIMER OS_TIMER_t;

//! timer callback type
typedef void (* OS_TIMERFUNC_t)(void *);

//! arm single-shot timer
/*!
    \param[in] pTimer   the timer
    \param[in] cb       callback function
    \param[in] pArg     argument to the callback function
    \param[in] timeout  timeout in system ticks
    \param[in] repeat   repeat timer every this often (or 0 for single-shot timer)
*/
void osTimerArm(OS_TIMER_t *pTimer, OS_TIMERFUNC_t cb, void *pArg, uint32_t timeout, uint16_t repeat);

//! kill timer
/*!
    \param[in] pTimer  the timer
*/
bool osTimerKill(OS_TIMER_t *pTimer);

//@}

/* ************************************************************************** */
/*!
    \name Queue Functions

    See also \ref ATOM_QUEUE.

    \warning Not tested!

    @{
*/

//! a queue
typedef ATOM_QUEUE OS_QUEUE_t;

void osQueueCreate(OS_QUEUE_t *pQueue, void *pBuf, const uint16_t length, const uint16_t itemSize);
bool osQueueSend(OS_QUEUE_t *pQueue, const void *pkMsg, const int32_t timeout);
bool osQueueReceive(OS_QUEUE_t *pQueue, const int32_t timeout, void *pMsg);
void osQueueDelete(OS_QUEUE_t *pQueue);

//@}

/* ************************************************************************** */

#endif // __OS_H__

//@}
// eof

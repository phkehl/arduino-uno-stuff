/**
    \file

    \brief Atom Threads Library (see \ref ATOM)

    \defgroup ATOM Atom Threads

    The library consists of the following modules:
    - \ref ATOM_KERNEL
    - \ref ATOM_QUEUE
    - \ref ATOM_TIMER
    - \ref ATOM_MUTEX
    - \ref ATOM_SEM
    - \ref ATOM_PORT

    @{

*/

#ifndef __ATOMTHREADS_H__
#define __ATOMTHREADS_H__

#include "atomtypes.h"


/* ***** atomkernel.c ******************************************************** */

uint8_t atomOSInit (void *idle_thread_stack_bottom, uint16_t idle_thread_stack_size, uint8_t idle_thread_stack_check);
void atomOSStart (void);
void atomSched (uint8_t timer_tick);
void atomIntEnter (void);
void atomIntExit (uint8_t timer_tick);
ATOM_TCB *atomCurrentContext (void);
uint8_t atomThreadCreate (ATOM_TCB *tcb_ptr, uint8_t priority, void (*entry_point)(uint32_t), uint32_t entry_param, void *stack_bottom, uint16_t stack_size, uint8_t stack_check);
uint8_t atomThreadStackCheck (const ATOM_TCB *tcb_ptr, uint16_t *used_bytes, uint16_t *free_bytes);
uint8_t tcbEnqueuePriority (ATOM_TCB **tcb_queue_ptr, ATOM_TCB *tcb_ptr);
ATOM_TCB *tcbDequeueHead (ATOM_TCB **tcb_queue_ptr);
ATOM_TCB *tcbDequeueEntry (ATOM_TCB **tcb_queue_ptr, ATOM_TCB *tcb_ptr);
ATOM_TCB *tcbDequeuePriority (ATOM_TCB **tcb_queue_ptr, uint8_t priority);
ATOM_TCB *atomGetIdleTCB(void);

/* ***** atomtimer.c ******************************************************** */

uint8_t atomTimerRegister (ATOM_TIMER *timer_ptr);
uint8_t atomTimerCancel (ATOM_TIMER *timer_ptr);
uint8_t atomTimerDelay (uint32_t ticks);
uint32_t atomTimeGet (void);
void atomTimeSet (uint32_t new_time);
void atomTimerTick (void);

/* ***** atomqueue.c ******************************************************* */

uint8_t atomQueueCreate (ATOM_QUEUE *qptr, uint8_t *buff_ptr, uint32_t unit_size, uint32_t max_num_msgs);
uint8_t atomQueueDelete (ATOM_QUEUE *qptr);
uint8_t atomQueueGet (ATOM_QUEUE *qptr, int32_t timeout, uint8_t *msgptr);
uint8_t atomQueuePut (ATOM_QUEUE *qptr, int32_t timeout, const uint8_t *msgptr);

/* ***** atomsem.c ******************************************************** */

uint8_t atomSemCreate (ATOM_SEM *sem, uint8_t initial_count);
uint8_t atomSemDelete (ATOM_SEM *sem);
uint8_t atomSemGet (ATOM_SEM *sem, int32_t timeout);
uint8_t atomSemPut (ATOM_SEM *sem);
uint8_t atomSemResetCount (ATOM_SEM *sem, uint8_t count);

/* ***** atommutex.c ******************************************************** */

uint8_t atomMutexCreate (ATOM_MUTEX *mutex);
uint8_t atomMutexDelete (ATOM_MUTEX *mutex);
uint8_t atomMutexGet (ATOM_MUTEX *mutex, int32_t timeout);
uint8_t atomMutexPut (ATOM_MUTEX *mutex);

#endif /* __ATOMTHREADS_H__ */

//@}

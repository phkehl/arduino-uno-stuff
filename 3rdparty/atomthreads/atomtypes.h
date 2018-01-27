/**
    \file

    \brief Atom Threads Library Types (see \ref ATOM)

    \addtogroup ATOM
    @{
*/

#ifndef __ATOMTYPES_H__
#define __ATOMTYPES_H__

#include <stdint.h>
#include <stddef.h>

/* ***** atomtimer.c ******************************************************* */

#define POINTER void *

/* Callback function prototype */
typedef void ( * TIMER_CB_FUNC ) ( POINTER cb_data ) ;

/* Timer descriptor */
typedef struct atom_timer
{
    TIMER_CB_FUNC   cb_func;    /* Callback function */
    POINTER	        cb_data;    /* Pointer to callback parameter/data */
    uint32_t	    cb_ticks;   /* Ticks until callback */

#ifdef ATOM_FLIPFLIP
    uint16_t cb_repeat;
#endif

	/* Internal data */
    struct atom_timer *next_timer;		/* Next timer in doubly-linked list */

} ATOM_TIMER;

/* ***** atomkernel.c ****************************************************** */

typedef struct atom_tcb
{
    /*
     * Thread's current stack pointer. When a thread is scheduled
     * out the architecture port can save its stack pointer here.
     */
    POINTER sp_save_ptr;

    /* Thread's port specific private data */
    //THREAD_PORT_PRIV;

    /* Thread priority (0-255) */
    uint8_t priority;

    /* Thread entry point and parameter */
    void (*entry_point)(uint32_t);
    uint32_t entry_param;

    /* Queue pointers */
    struct atom_tcb *prev_tcb;    /* Previous TCB in doubly-linked TCB list */
    struct atom_tcb *next_tcb;    /* Next TCB in doubly-linked list */

    /* Suspension data */
    uint8_t suspended;            /* TRUE if task is currently suspended */
    uint8_t suspend_wake_status;  /* Status returned to woken suspend calls */
    ATOM_TIMER *suspend_timo_cb;  /* Callback registered for suspension timeouts */
    uint8_t terminated;           /* TRUE if task is being terminated (run to completion) */

    /* Details used if thread stack-checking is required */
#ifdef ATOM_STACK_CHECKING
    POINTER stack_bottom;         /* Pointer to bottom of stack allocation */
    uint16_t stack_size;          /* Size of stack allocation in bytes */
#endif

#ifdef ATOM_FLIPFLIP
    uint16_t runtime; // runtime statistics
    const char *name;    // task name
    struct atom_tcb *next;
#endif // ATOM_FLIPFLIP

} ATOM_TCB;

/* ***** atomqueue.c ******************************************************* */

typedef struct atom_queue
{
    ATOM_TCB *  putSuspQ;       /* Queue of threads waiting to send */
    ATOM_TCB *  getSuspQ;       /* Queue of threads waiting to receive */
    uint8_t *   buff_ptr;       /* Pointer to queue data area */

#ifdef ATOM_FLIPFLIP
    uint8_t    unit_size;      /* Size of each message */
    uint8_t    max_num_msgs;   /* Max number of storable messages */
    uint8_t    insert_index;   /* Next byte index to insert into */
    uint8_t    remove_index;   /* Next byte index to remove from */
    uint8_t    num_msgs_stored;/* Number of messages stored */
#else
    uint32_t    unit_size;      /* Size of each message */
    uint32_t    max_num_msgs;   /* Max number of storable messages */
    uint32_t    insert_index;   /* Next byte index to insert into */
    uint32_t    remove_index;   /* Next byte index to remove from */
    uint32_t    num_msgs_stored;/* Number of messages stored */
#endif
} ATOM_QUEUE;

/* ***** atomsem.c ******************************************************** */

typedef struct atom_sem
{
    ATOM_TCB *  suspQ;  /* Queue of threads suspended on this semaphore */
    uint8_t     count;  /* Semaphore count */
} ATOM_SEM;

/* ***** atommutex.c ******************************************************** */

typedef struct atom_mutex
{
    ATOM_TCB *  suspQ;  /* Queue of threads suspended on this mutex */
    ATOM_TCB *  owner;  /* Thread which currently owns the lock */
    uint8_t     count;  /* Recursive count of locks by the owner  */
} ATOM_MUTEX;



/* Constants */
#define TRUE                    1
#define FALSE                   0

/* Error values */

#define ATOM_OK                 0
#define ATOM_ERROR              1
#define ATOM_TIMEOUT            2
#define ATOM_WOULDBLOCK         3
#define ATOM_ERR_CONTEXT        200
#define ATOM_ERR_PARAM          201
#define ATOM_ERR_DELETED        202
#define ATOM_ERR_OVF            203
#define ATOM_ERR_QUEUE          204
#define ATOM_ERR_TIMER          205
#define ATOM_ERR_NOT_FOUND      206
#define ATOM_ERR_OWNERSHIP      207

//! Idle thread priority (lowest)
#define IDLE_THREAD_PRIORITY    9
//#define IDLE_THREAD_PRIORITY    255



#endif /* __ATOMTYPES_H__ */

//@}

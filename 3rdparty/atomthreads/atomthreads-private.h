#include "atomport.h"

#ifndef __ATOMTHREADS_PRIVATE_H__
#define __ATOMTHREADS_PRIVATE_H__

/* Global data */
extern ATOM_TCB *tcbReadyQ;
extern uint8_t atomOSStarted;

/* Constants */
#define TRUE                    1
#define FALSE                   0

// in atomport-asm.s
extern void archContextSwitch (ATOM_TCB *old_tcb_ptr, ATOM_TCB *new_tcb_ptr);
extern void archFirstThreadRestore(ATOM_TCB *new_tcb_ptr);
// in atomport.c
extern void archThreadContextInit (ATOM_TCB *tcb_ptr, void *stack_top, void (*entry_point)(uint32_t), uint32_t entry_param);

#endif /* __ATOMTHREADS_PRIVATE_H__ */

//@}

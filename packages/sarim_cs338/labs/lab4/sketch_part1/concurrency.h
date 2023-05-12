/*************************************************************************
 *
 *  Copyright (c) 2013--2018 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__


#ifdef __cplusplus
extern "C" {
#endif

/* opaque definition of process type; you must provide this
   implementation.
*/
typedef struct process_state {
    unsigned int sp;            /* stack pointer */
    struct process_state* next; /* link to next process */
} process_t;

// also defined a manager for the many processes
typedef struct process_state_manager {
    process_t* head;
    int count;
} psm;

/*------------------------------------------------------------------------

   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.

------------------------------------------------------------------------*/

/* ====== Part 1 ====== */

extern process_t* current_process; 
/* the currently running process */

extern psm* global_manager;

__attribute__((used)) unsigned int process_select(unsigned int cursp);
/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/

void process_start(void);
/* Starts up the concurrent execution */

int process_create(void (*f)(void), int n);
/* Create a new process */

/* ===== Part 2 ====== */

typedef struct lock_state lock_t;
/* you have to define the lock_state structure */

void lock_init(lock_t* l);
void lock_acquire(lock_t* l);
void lock_release(lock_t* l);

/*-- functions provided in the .c file --*/

unsigned int process_init(void (*f)(void), int n);
void process_begin();
void yield();

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */




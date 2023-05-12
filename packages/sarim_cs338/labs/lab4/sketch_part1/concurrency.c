
#include "concurrency.h"
#include "helper.hpp"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

// ** part 1

// declare the extern variable
process_t* current_process; 
// global manager
psm* global_manager;

int process_create(void (*f)(void), int n) {
    disableInterruptsWrapper();

    unsigned int stk = process_init(f, n);
    if (stk == 0) {  return -1; }

    // create ready queue if not done already
    if (global_manager == NULL) {
        global_manager = psmCreate();
        if (global_manager == NULL) { return -1; }
    }

    // add to the queue and return
    psmPushToBack(global_manager, stk);

    enableInterruptsWrapper();
    return 0;
}

void process_start(void) {
    // initialize any needed data structures
    current_process = NULL;

    // call process_begin (which will invoke process_select)
    process_begin();
}

unsigned int process_select(unsigned int cursp) {
    // if no queue or no processes, return 
    if (global_manager == NULL) { return 0; }
    if (global_manager->count < 1) { return 0; }

    if (cursp == 0) {

        // start new
        if (current_process == NULL) {
            // remove from front, add back, and designate as current process
            unsigned int sp = psmPop(global_manager);
            psmPushToFront(global_manager, sp);
            current_process = psmFind(global_manager, sp);
            // return sp of current
            return sp;
        }

        // terminated
        if (current_process != NULL) {

            // get rid of the current process at front
            current_process = NULL;
            psmPop(global_manager);

            // see if any processes left
            if (global_manager->count == 0) {
                psmDestroy(&global_manager);
                global_manager = NULL;
                return 0;
            }
        }

    // if cursp != 0
    } else  {
        // "update" current process and send to back
        psmPop(global_manager);
        psmPushToBack(global_manager, cursp);
    }

    // choose new as current
    unsigned int sp = psmPop(global_manager);
    psmPushToFront(global_manager, sp);
    current_process = psmFind(global_manager, sp);
    return sp;
}

__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;

__attribute__((used)) void process_begin() {
    asm volatile(
        "cli \n\t"                          // clear global interrupt flag
        "in r24,__SP_L__ \n\t"              // load stack pointer lower byte into r24
        "sts _orig_sp_lo, r24\n\t"          // store register containing stack pointer lower byte to SRAM (data space)
        "in r25,__SP_H__ \n\t"              // do the same for the higher byte
        "sts _orig_sp_hi, r25\n\t"
        "ldi r24, 0\n\t"                    // clear r24 and r25
        "ldi r25, 0\n\t"        
        "rjmp .dead_proc_entry\n\t");       // jump to within process_timer_interrupt(), which will soon call process_select()           
}

__attribute__((used)) void process_terminated() {
    asm volatile(
        "cli\n\t"                           // disable interrupts
        "lds r25, _orig_sp_hi\n\t"          // load direct from data space/SRAM into r25 (which is the saved stack pointer)
        "out __SP_H__, r25\n\t"             // restore stack pointer to the saved stack pointer
        "lds r24, _orig_sp_lo\n\t"          
        "out __SP_L__, r24\n\t"
        "ldi r24, lo8(0)\n\t"               // clear r24 and r25
        "ldi r25, hi8(0)\n\t"               
        "rjmp .dead_proc_entry");           // jump to within process_timer_interrupt(), which will soon call process_select()        
}

void process_timer_interrupt();

// i think this means the thread is not doing anything important and should switch to another 
// yield() is inside the applied patch to the Arduino source
// it calls process_timer_interrupt() => process_select() periodically 
__attribute__((used)) void yield() {
    if (!current_process) return;                       // if current process is NULL, do nothing
    asm volatile("cli\n\t");                            // otherwise disable interrupts
    asm volatile("rjmp process_timer_interrupt\n\t");   // jump to start of process_timer_interrupt() 
}

__attribute__((used)) void process_timer_interrupt() {
    asm volatile(
        "push r31\n\t"                      // presumably save all registers
        "push r30\n\t"                      
        "push r29\n\t"
        "push r28\n\t"
        "push r27\n\t"
        "push r26\n\t"
        "push r25\n\t"
        "push r24\n\t"
        "push r23\n\t"
        "push r22\n\t"
        "push r21\n\t"
        "push r20\n\t"
        "push r19\n\t"
        "push r18\n\t"
        "push r17\n\t"
        "push r16\n\t"
        "push r15\n\t"
        "push r14\n\t"
        "push r13\n\t"
        "push r12\n\t"
        "push r11\n\t"
        "push r10\n\t"
        "push r9\n\t"
        "push r8\n\t"
        "push r7\n\t"
        "push r6\n\t"
        "push r5\n\t"
        "push r4\n\t"
        "push r3\n\t"
        "push r2\n\t"
        "push r1\n\t"
        "push r0\n\t"
        "in r24, __SREG__\n\t"              // r24 = actual status register
        "push r24\n\t"                      // save above to stack frame
        "in r24, __SP_L__\n\t"              // store the actual stack pointer to r24:r25
        "in r25, __SP_H__\n\t"          
        ".dead_proc_entry:\n\t"             // this is a label for dead_proc_entry (dead process entry?)
        "rcall process_select\n\t"          // which calls process_select, which then returns here with r24:r25 (probably the sp from process_select?)
        "eor r18,r18\n\t"                   // clear r18 (exclusive or with itself)
        "or r18,r24\n\t"                    // set bits of r18 to r24? 
        "or r18,r25\n\t"                    // and r25?
        "brne .label_resume\n\t"            // if r18 ends up being non-zero, the program jumps to label resume
        "lds r25, _orig_sp_hi\n\t"          // if r18 is 0
        "out __SP_H__, r25\n\t"             //      actual stack pointer = stack pointer from SRAM (restored)
        "lds r24, _orig_sp_lo\n\t"          
        "out __SP_L__, r24\n\t"            
        "ret\n\t"                           //      return 
        ".label_resume:\n\t"                // else if r18 = r24:r25 is non-zero 
        "out __SP_L__, r24\n\t"             //      actual stack pointer = r24:r25
        "out __SP_H__, r25\n\t"             
        "pop r0\n\t"                        // restore SREG
        "out  __SREG__, r0\n\t"             // restore all other registers
        "pop r0\n\t"
        "pop r1\n\t"
        "pop r2\n\t"
        "pop r3\n\t"
        "pop r4\n\t"
        "pop r5\n\t"
        "pop r6\n\t"
        "pop r7\n\t"
        "pop r8\n\t"
        "pop r9\n\t"
        "pop r10\n\t"
        "pop r11\n\t"
        "pop r12\n\t"
        "pop r13\n\t"
        "pop r14\n\t"
        "pop r15\n\t"
        "pop r16\n\t"
        "pop r17\n\t"
        "pop r18\n\t"
        "pop r19\n\t"
        "pop r20\n\t"
        "pop r21\n\t"
        "pop r22\n\t"
        "pop r23\n\t"
        "pop r24\n\t"
        "pop r25\n\t"
        "pop r26\n\t"
        "pop r27\n\t"
        "pop r28\n\t"
        "pop r29\n\t"
        "pop r30\n\t"
        "pop r31\n\t"
        "reti\n\t");                    // return from the interrupt (sets interrupts and stack pointer again)
}

/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init(void (*f)(void), int n) {
    unsigned long stk;
    int i;
    unsigned char* stkspace;

    /* Create a new process */
    n += EXTRA_SPACE + EXTRA_PAD;
    stkspace = (unsigned char*)malloc(n);

    if (stkspace == NULL) {
        /* failed! */
        return 0;
    }

    /* Create the "standard" stack, including entry point */
    for (i = 0; i < n; i++) {
        stkspace[i] = 0;
    }

    n -= EXTRA_PAD;

    stkspace[n - 1] = ((unsigned int)process_terminated) & 0xff;
    stkspace[n - 2] = ((unsigned int)process_terminated) >> 8;
    stkspace[n - 3] = ((unsigned int)f) & 0xff;
    stkspace[n - 4] = ((unsigned int)f) >> 8;

    /* SREG */
    stkspace[n - EXTRA_SPACE] = SREG;

    stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;

    return stk;
}
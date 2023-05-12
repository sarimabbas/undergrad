#include "testasm.h"

/* the attribute "used" tells the compiler to not inline this function */
__attribute__((used))
unsigned int testasm(unsigned char a, unsigned char b) {
  unsigned int out;

  asm volatile(R"(

   rjmp start_of_assembly

   .data
   ; any data values/static storage can go here

   .text

start_of_assembly:
    ; Your assembly language program goes here
    ;
    ; For this example, your program must not modify any registers other than 
    ; r24 and r25. In other words, save and restore all the registers that
    ; are modified by your code.

    ; Tell the compiler to move the arguments a, b into registers 
    ; r24, r25 respectively
    ;
    mov r24,%1
    mov r25,%2
    ;
    ;  --- YOUR CODE GOES HERE ---
    ;       r24 = a, r25 = b
    ;

    fibonacci:
        ; prolog
        clr r25         ; r25 is not needed
        push r16        ; 16, 17, and 18 cycle through the fibonacci sequence
        push r17
        push r18

        ; body
        ldi r16, 0      ; init with base cases
        ldi r17, 1
        
        cpi r24, 0 
        breq epilog
        cpi r24, 1
        breq epilog

        call loop       ; f(n) -> r24

        ; epilog
        epilog:
        pop r18
        pop r17
        pop r16
        rjmp end_of_assembly

    loop:
        mov r18, r17    ; the result ends up in r17 as the values are cycled
        add r17, r16
        mov r16, r18

        subi r24, 1     ; check when to stop
        cpi r24, 1
        brne rep

        mov r24, r17    ; store final result
        ret

        rep:
        call loop
        
    ;
    ;  --- YOUR CODE ENDS ---
    ;      The result must be in the register pair r25:r24
    ;      You can also rjmp to the label end_of_assembly if you need to.
    ;
end_of_assembly: 
   ; -- move r25:r24 to the 16-bit word in variable out
   movw %0,r24

)"
               : "=w"(out) /* out is %0, and a 16-bit variable that is modified
                              by the assembly language */
               : "r"(a), "r"(b) /* a is %1, b is %2, both are register-sized
                                   variables */
               : "r25", "r24"); /* r24, r25 are modified by this assembly
                                   language fragment */

  return out;
}

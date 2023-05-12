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

start:
    push r20            ; to store result of diffval

    call diffval        ; call diffval -> r20
    call sumval         ; call sumval -> r25:r24
    add r24, r20

    pop r20         
    rjmp end_of_assembly

diffval:
    push r16            ; copy r24 and r25
    push r17        
    mov r16, r24        ; arg a
    mov r17, r25        ; arg b

    cp r17, r16         ; compare b with a

    brsh samehigher
    sub r16, r17        ; a > b
    mov r20, r16        ; load into result
    rjmp epilog         ; finished

    samehigher:         
    sub r17, r16        ; b >= a
    mov r20, r17        ; load into result

    epilog: pop r17
    pop r16
    ret

sumval: 
    push r16            ; copy r24 and r25 into two new registers
    push r17  
    mov r16, r24        ; arg a
    mov r17, r25        ; arg b

    add r16, r17        ; add r17 into r16
    clr r17             ; set all the bits of r17 to 0
    adc r17, r1         ; add the zero-reg + carry into r17
    movw r24, r16       ; move r17:r16 to the 16-bit word in r25:r24

    pop r17
    pop r16
    ret

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

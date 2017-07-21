/*----------------------------------------------------------------------------
LAB EXERCISE - SQUARE ROOT APPROXIMATION
 ----------------------------------------
Write an assembly code subroutine to approximate the square root of an 
argument using the bisection method. All math is done with integers, so the 
resulting square root will also be an integer

GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include <STM32F4xx.h>

__asm int sqrt_approx(int x)
	{
	
	//Write your code here
	
	   push {r4,r5}
	   ldr r1, =0
	   ldr r2, =0xffff
	   ldr r3, =0xffffffff
	
Loop
    	mov  r5, r3
	    adds r3, r1, r2
		  asrs r3, #1
		  mov  r4, r3
		  muls r4, r3, r4
		  cmp r4, r0
		  beq Done
      bgt Higher
      mov r1, r3
      b   Cont
Higher
       mov r2, r3
Cont 
       cmp r5, r3
			 bne Loop
Done 
       mov r0, r3
			 pop {r4, r5}
			 bx   lr
}
	    



/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main(void){
	volatile int a,b,c,d, k=0;
	int i;
  a = sqrt_approx(0);     // should be 0
  b = sqrt_approx(25);    // should be 5
	c = sqrt_approx(133); 	// should be 11
	
  for (i=0; i<10000; i++){
		d = sqrt_approx(i);
    k+=d;
  }
	while(1)
		;
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2016*************************************

/*----------------------------------------------------------------------------
LAB EXERCISE - LED blinky with interrupts
 ----------------------------------------
	In this exercise we will create a program which uses a button to turn on and off a blinky LED.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"

// Create a DigitalOut objects for the LED
DigitalOut Led_out(LED1);


// Create an InterruptIn object for the button. 
InterruptIn button_press(USER_BUTTON);


// Create a recurring interruption.
Ticker blinky;

//Button handler
void button_ISR(){
Led_out = !Led_out;
}

//Recurrent handler
void blink(){
Led_out = !Led_out;
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main() {
// Set up
	bool onf = false;
	button_press.rise(&button_ISR);
while(true)
{
  
 //Interrupt sub-routine button_ISR will be called
//when a rising edge occurs.
 //waiting for interrupts

   if(!button_press)
	 {
				if(!onf)
				{
			   blinky.attach(&blink, 0.5);
			   onf = true; 
				 
				}
			
			
			else
			{	
				blinky.detach();
				Led_out=0;
				onf = false;
				Led_out=0;
			}
			
	}
 }
}


			 
       			 



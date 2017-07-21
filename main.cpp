/*----------------------------------------------------------------------------
LAB EXERCISE - Simple LED blinky
 ----------------------------------------
In this exercise we will create a program that blinks a LED while the user button is being pressed.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"

// Create a DigitalOut objects for the LED
DigitalOut Led_out(LED1);


// Create a DigitalIn objects for the button. 
DigitalIn button_press(USER_BUTTON);

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main() {

	while(1) {
		// The buttons is active low
		Led_out = 0;
		
    // If the button is pressed the LED blinks twice per second
    if (!button_press)
		{
    Led_out = 1; //toggle the state of Led_out
		wait(0.25);
		Led_out = 0;
		wait(0.25);
		}
		// Otherwise the LED is switch off.
    else 
			Led_out=0;
  }
}

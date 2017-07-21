/*----------------------------------------------------------------------------
LAB EXERCISE - Nucleo sensors shield
 ----------------------------------------
	In this exercise we will read the environmental sensors on the Nucleo sensors shield (X-NUCLEO-IKS01A1)
	with the help of the ST Nucleo Sensor Shield library which is compatible with the MBED API.
	Then we will send the measurements via USB to our PC using serial communication.
	We can then display the results using a terminal emulation program (e.g. Termite).

	At the same time the program blinks the on board LED to show alivness.


	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "x_cube_mems.h"
DigitalOut led1(LED1);

// Create a Serial objects to communicate via USB
Serial pc(SERIAL_TX, SERIAL_RX);

float humidityvalue;
float pressurevalue;
float Cel;
float Far;
float kel;
AxesRaw_TypeDef MAG_Value;
AxesRaw_TypeDef ACC_Value;
AxesRaw_TypeDef GYR_Value;
Ticker ticker;
int n=0;
//int a[10];
//int i;
//int j;

void call(void)
{
    led1 = !led1; 
}

int main() {
static X_CUBE_MEMS *mems_expansion_board = X_CUBE_MEMS::Instance();
   ticker.attach(call, 0.5); 
while(n==10) {
			  mems_expansion_board->hts221.GetTemperature((float *)&Cel);
        mems_expansion_board->hts221.GetHumidity((float *)&humidityvalue);
        mems_expansion_board->lps25h.GetPressure((float *)&pressurevalue);
     
    

		//kel=(Cel + 459.67) * 0.555f;
		Cel = (Cel * 1.8f) + 32.0f;

		//pc.printf("Temp:%f C / %f F %f K \t", Cel, Far,kel);
	  pc.printf("Temp:%f C \n \r", Cel);
	   
	   if (Cel>70 && Cel<75)
		 {
			 pc.printf("The temperature is normal\n \r");
			//a[i]=Cel;
		 }
	  else
		{
			//a[j]=Cel;
			pc.printf("The temperature is abnormal\n \r");
		}
	
  	pc.printf("Humidity %f \n \r ", humidityvalue);
	  pc.printf("Pressure %f \n \r", pressurevalue);
		wait(3);
		n++;	
	//pc.printf("Frequency of normal temperature is %d\n \r", i );
	//pc.printf("Frequency of abnormal temperature is %d\n \r", j );
   }
  //pc.printf("Frequency of normal temperature is %d\n \r", i );
	//pc.printf("Frequency of abnormal temperature is %d\n \r", j );
 }
// *******************************ARM University Program Copyright © ARM Ltd 2016*************************************


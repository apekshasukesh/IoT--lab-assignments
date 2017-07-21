
/*----------------------------------------------------------------------------
LAB EXERCISE - Weather Station Appcessory
 ----------------------------------------
	In this exercise we will create a BLE weather station appcessory that will measure several environmental parameters and
	send them to our smartphone.
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "x_cube_mems.h"
#include "BLEDevice.h"
#include "EnvironmentalSensingService.h" //Complete the Environmental Sensing Services
#include "EnvironmentalSensingService2.h"
#include "DeviceInformationService.h"
#include "Utils.h"/* To turn on/off the debug messages on the console edit this file; NEED_CONSOLE_OUTPUT 1/0( it will have an impact on code-size and power consumption.) */
#define PI 3.1415169f
BLEDevice  ble;
EnvironmentalSensingService *environmentalSensingService;
EnvironmentalSensingService2 *environmentalSensingService2;
DeviceInformationService *deviceInfo;
float x;
float y;

DigitalOut led(LED1);
Ticker update;
Ticker aliveness;
Serial pc(SERIAL_TX, SERIAL_RX);

volatile float TEMPERATURE_C = 20;
volatile float HUMIDITY = 50;
volatile float PRESSURE = 1000;
volatile float WIND_DIRECTION = 0;
int16_t MagRaw[3];
volatile AxesRaw_TypeDef MAG_Value;

const static char     DEVICE_NAME[]        = "APEKSHA"; //Rename your device (short name)
static const uint16_t uuid16_list[]        = {GattService::UUID_ENVIRONMENTAL_SENSING_SERVICE};

static volatile bool  triggerSensorPolling = false;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    DEBUG("Disconnected!\n\r");
    DEBUG("Restarting the advertising process\n\r");
    ble.startAdvertising(); // restart advertising
}

void connectionCallback(Gap::Handle_t handle, const Gap::ConnectionParams_t *reason)
{    DEBUG("Connected\r\n");}

void update_handler(void)
{    triggerSensorPolling = true;}

void aliveness_handler (void)
{	led = !led;	}

	

int main(void)
{
	int32_t axes[3];
	  /* Create a sensor shield object */
    static X_CUBE_MEMS *Sensors = X_CUBE_MEMS::Instance();

    update.attach(update_handler, 2);
	  aliveness.attach(aliveness_handler,0.5);

    DEBUG("Initialising \n\r");
    ble.init();
    ble.onDisconnection(disconnectionCallback);
    ble.onConnection(connectionCallback);

	  /* Setup primary services. */

		environmentalSensingService = new EnvironmentalSensingService(ble,HUMIDITY ,TEMPERATURE_C);
		environmentalSensingService2 = new EnvironmentalSensingService2(ble,WIND_DIRECTION ,PRESSURE);

    /* Setup auxiliary services. */

				deviceInfo = new DeviceInformationService(ble, "STM", "Nucleo32", "SN1");

    /* Setup advertising. */
		ble.accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.accumulateAdvertisingPayload(GapAdvertisingData::MY_WEATHER_STATION);
    ble.accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.setAdvertisingInterval(1600); /* 1000ms; in multiples of 0.625ms. */
    ble.startAdvertising();

    while (true)
    {
        if (triggerSensorPolling & ble.getGapState().connected)
        {
				 /* Read the environmental sensors and the magnetometer */
						Sensors->hts221.GetTemperature((float *)&TEMPERATURE_C);
						Sensors->hts221.GetHumidity((float *)&HUMIDITY);
						Sensors->lis3mdl.GetAxes((AxesRaw_TypeDef *) &MAG_Value);
						Sensors->lps25h.GetPressure((float *)&PRESSURE);
					

       /*pc.printf("Temperature:\t\t %f F / %f K\r\n", TEMPERATURE_C);
        pc.printf("Humidity:\t\t %f%%\r\n", HUMIDITY);
        pc.printf("Pressure:\t\t %f hPa\r\n", PRESSURE); 
				pc.printf("Magnetometer (mGauss):\t X: %f, Y: %f, Z: %f\r\n", MAG_Value.AXIS_X, MAG_Value.AXIS_Y, MAG_Value.AXIS_Z);	
      
        pc.printf("\r\n"); */
					
					
				 /* Calcule the direction where the system is pointing  relative to North with the magnetometer measurments.*/
						if (MAG_Value.AXIS_X < 140) WIND_DIRECTION = 0; //North
                    else if (MAG_Value.AXIS_X >= 140 && MAG_Value.AXIS_X < 200 && -MAG_Value.AXIS_Y > 250 ) WIND_DIRECTION = 45;  //Northeast
											else if (MAG_Value.AXIS_X >= 140 && MAG_Value.AXIS_X < 200 && -MAG_Value.AXIS_Y < 250 ) WIND_DIRECTION = 315; //Northwest
                    else if (MAG_Value.AXIS_X >= 200 && MAG_Value.AXIS_X < 280 && -MAG_Value.AXIS_Y > 250 ) WIND_DIRECTION = 90;  //East
                    else if (MAG_Value.AXIS_X >= 200 && MAG_Value.AXIS_X < 280 && -MAG_Value.AXIS_Y < 250 ) WIND_DIRECTION = 270; //Weast
                    else if (MAG_Value.AXIS_X >= 280 && MAG_Value.AXIS_X < 360 && -MAG_Value.AXIS_Y > 250 ) WIND_DIRECTION = 135; //Southeast
                    else if (MAG_Value.AXIS_X >= 280 && MAG_Value.AXIS_X < 360 && -MAG_Value.AXIS_Y < 250 ) WIND_DIRECTION = 225; //Soutwest           
                    else if (MAG_Value.AXIS_X >= 360) WIND_DIRECTION = 180; //South

					    

					/* Multiply by 10^Number of decimals that needs to be pass to the client acordingly to the standards */

							TEMPERATURE_C = (uint16_t)(TEMPERATURE_C *100);
							HUMIDITY =(uint16_t)(HUMIDITY *100);
							PRESSURE = (uint32_t)(PRESSURE *1000);
							WIND_DIRECTION =(uint16_t)(WIND_DIRECTION *100);
					/* Update the characteristics */ 

							environmentalSensingService->updateTemperature(TEMPERATURE_C);
            environmentalSensingService2->updatePressure(PRESSURE);
            environmentalSensingService->updateHumidity(HUMIDITY);
						environmentalSensingService2->updateWinddirection(WIND_DIRECTION);

					triggerSensorPolling = false;
        }
        else
        {
            ble.waitForEvent();
        }
    }
}

// *******************************ARM University Program Copyright © ARM Ltd 2016*************************************//

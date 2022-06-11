/*
* XY MOTOR CONTROL PROJECT
* @JAUHAR K
*/

// the setup function runs once when you press reset or power the board

#include "m_LEDIndicator.h"
#include "m_InterruptService.h"
#include "m_sensorControl.h"
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "m_modBusComm.h"
#include "m_MotorDrive.h"


/* SETUP -----------------------------------*/
void setup() {



	Serial.begin(9600); 
	Serial.println("PROGRAM STARTING... "); 


	/* IR Sensor Initialisation */
	if (ir_Sensor_1.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR 1");
		Serial.println("CHECK THE SENSORS");
		while (1);
	}
	else Serial.println("SUCCESSFULLY INITIALISED SENSOR 1");

	/* Motor Initialisation */
	Motor1.motorInit();
	Motor1.enableMotor();

	/* Connecting modbus to client */
	if (myController.connectToClient()) {
		Serial.println("FAILED TO CONNECT!");
		while (1);
	}
	else Serial.println("CONNECTED TO CLIENT");

	setupLEDIndicators(); 


	/*
	Setting timer interrrupt for reading data every 1ms
	1 total reading will be in 10mS
	->timer interrupt 1000hz
	*/
	setTimerInterrupt();

}

/* LOOP -------------------------------------*/

void loop() {

	myController.poll(); 
	ModbusRTUServer.holdingRegisterWrite(4, ir_Sensor_1.getCount());
	
	if (myController.getCtrlData(0) > 0)
	{
		ledIndicator_ON(motorCW_LED); 
		Motor1.cRotate();
	}
	else { 
		ledIndicator_OFF(motorCW_LED); 
		Motor1.stop_cRotate(); 
	}
	
	if (myController.getCtrlData(2) > 0)
	{
		ledIndicator_ON(motorCCW_LED); 
		Motor1.ccRotate();
	}
	else {
		ledIndicator_OFF(motorCCW_LED); 
		Motor1.stop_ccRotate();
	}

}

/* EVENT ------------------------------------*/

void serialEvent2() {
	//statements
	myController.readFromClient();
}

/* ISR --------------------------------------*/

ISR(TIMER1_COMPA_vect) {
	ir_Sensor_1.updateData(); 
	toggleLedIndicator(timerInterrupt_LED); 
	TCNT1 = 0; 
}
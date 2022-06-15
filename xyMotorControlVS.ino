/*
* XY MOTOR CONTROL PROJECT
* @JAUHAR K
*/

// the setup function runs once when you press reset or power the board

#include <NoDelay.h>
#include "m_LEDIndicator.h"
#include "m_InterruptService.h"
#include "m_sensorControl.h"
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>
#include "m_modBusComm.h"
#include "m_MotorDrive.h"


/* ============================================================== */
/* SETUP -----------------------------------*/

int PROG_INITIALISED = 0; 

/* ----------------------------------------- */

void setup() {
	/* Motor Initialisation */

	/* X direction motor */
	MotorXa.motorInit();
	MotorXa.enableMotor();

	MotorXb.motorInit();
	MotorXb.enableMotor();


	/* Initialisint sensor */
	if (ir_Sensor_1.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR 1");
		Serial.println("CHECK THE SENSORS");
		while (1);
	}
	else Serial.println("SUCCESSFULLY INITIALISED SENSOR 1");


	setupLEDIndicators(); 
	/* Connecting modbus to client */
	if (myController.connectToClient()) {
		Serial.println("FAILED TO CONNECT!");
		while (1);
	}
	else Serial.println("CONNECTED TO CLIENT");
	


	PROG_INITIALISED = 110; 
}

/* ============================================================== */
/* LOOP -------------------------------------*/

noDelay fun1Time(1);

void loop() 
{
	myController.poll(); 

	ir_Sensor_1.updateData(); 

	/* COMMUNICATION DATA UPDATE*/
	/*-------------------------------------------------*/
	/*	0: PROG INITIALISED? 
	*	1: MotorXa state
	*	2: MotorXa counter (irSensor1)
	*	3: MotorXb state
	*	4: MotorXb counter (irSensor2)
	*/

	myController.toClientData[0] = MotorXa.getState(); /*index 0 in labview*/
	myController.toClientData[1] = MotorXb.getState();
	myController.toClientData[2] = 231; 
	myController.toClientData[3] = ir_Sensor_1.getCount(); /*index 3 in labview*/

	myController.sendDataToClient(); 

	/* WHEN PRESSED FORWARD*/
	if (myController.getCtrlData() ==1) {
		/* ROTATE CW */
		ledIndicator_ON(IND_LED3); 
		MotorXa.cRotate(); 
		MotorXb.cRotate(); 
	}
	/* WHEN PRESSED REVERSE */
	else if (myController.getCtrlData()==3) {
		ledIndicator_OFF(IND_LED3);
		/* ROTATE CCW */
		ledIndicator_ON(IND_LED4); 
		MotorXa.ccRotate(); 
		MotorXb.ccRotate(); 
	}
	else {
		ledIndicator_OFF(IND_LED4); 
		ledIndicator_OFF(IND_LED3); 
		MotorXa.motorHALT(); 
		MotorXb.motorHALT(); 
	}
	

	toggleLedIndicator(IND_LED8); 
	delayMicroseconds(500); 
	/*-------------------------------------------------*/
}

/* ============================================================== */
/* EVENT ------------------------------------*/

void serialEvent2() {
	//statements
	myController.readFromClient();
}

/* ISR --------------------------------------*/
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
	while (ir_Sensor_1.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR 1"); 
		Serial.println("CHECK THE SENSORS"); 
	}
	Serial.println("SUCCESSFULLY INITIALISED SENSOR 1");

	/* Connecting modbus to client */
	while (myController.connectToClient()==false)
	{
		Serial.println("FAILED TO CONNECT TO MASTER"); 
	}
	Serial.println("CONNECTED TO CLIENT");
	

	setupLEDIndicators();

	PROG_INITIALISED = 110; 
}

/* ============================================================== */
/* LOOP -------------------------------------*/

noDelay fun1Time(10);

void loop() 
{
	myController.poll(); 

	/* COMMUNICATION DATA UPDATE*/
	/*-------------------------------------------------*/
	/*	0: PROG INITIALISED? 
	*	1: MotorXa state
	*	2: MotorXa counter (irSensor1)
	*	3: MotorXb state
	*	4: MotorXb counter (irSensor2)
	*/

	/*
	* 1 motor Rotation=11.14cm 
	*/
	myController.toClientData[0] = MotorXa.getState(); /*index 0 in labview*/
	myController.toClientData[1] = MotorXb.getState();
	myController.toClientData[2] =ir_Sensor_1.getCount();
	myController.toClientData[3] = ir_Sensor_1.getCount(); /*index 3 in labview*/

	myController.sendDataToClient(); 

	/* MOTOR CONTROLS */

	switch (myController.getCtrlData()) {
	case CTRL_UP: 
		ledIndicator_ON(IND_LED3); 
		if (ir_Sensor_1.getCount() >= X_MAX) {
			MotorXa.motorHALT(); 
			MotorXb.motorHALT(); 
			break;
		}
		MotorXa.cRotate();
		MotorXb.cRotate();
		ir_Sensor_1.updateData(1);
		break;
		
	case CTRL_DOWN: 
		ledIndicator_OFF(IND_LED3);
		ledIndicator_ON(IND_LED4); 
		if (ir_Sensor_1.getCount() <= X_ORIGIN) {
			MotorXa.motorHALT(); 
			MotorXb.motorHALT(); 
			break;
		}
		MotorXa.ccRotate(); 
		MotorXb.ccRotate(); 
		ir_Sensor_1.updateData(-1); 
		break;

	case CTRL_SET_ORIGIN: 
		ledIndicator_OFF(IND_LED3); 
		ledIndicator_OFF(IND_LED4); 
		ledIndicator_ON(IND_LED5); 
		ir_Sensor_1.resetSensor();

	default: 
		ledIndicator_OFF(IND_LED3); 
		ledIndicator_OFF(IND_LED4); 
		MotorXa.motorHALT(); 
		MotorXb.motorHALT(); 
	}



	/*	GO TO ORIGIN */
	/*
	* WHEN GO to Origin pressed (X_counter=0, Y_Counter=0), 
	* first, X motors rotates in CW direction until it presses button in X_Origin
	* then it resets the X_counter
	* Then Y motor rotates in CW direction until it presses button in Y_Origin
	* then it resets the Y_Counter.
	*/

	/* AVOID MOTOR LOCKUP */
	/*
	* WHEN Xa_Counter != Xb_Counter, then motor lockup occurs. 
	* To avoid error buildup, desired to rest motors at Origin (0,0); 
	* loopwise check values for Xa_Counter and Xb_Counter. if one is greater than other, 
	* then stop the other motor and slowly brings the other motor to position. 
	*/



	toggleLedIndicator(IND_LED1);
	delayMicroseconds(10); 
	/*-------------------------------------------------*/
}

/* ============================================================== */
/* EVENT ------------------------------------*/

void serialEvent2() {
	//statements
	myController.readFromClient();
}

/* ISR --------------------------------------*/
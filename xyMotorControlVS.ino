/*
* XY MOTOR CONTROL PROJECT
* @JAUHAR K
*/

// the setup function runs once when you press reset or power the board

#include "m_switches.h"
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

	/* initialising limiterSwitch for Xa */


	//pinMode(X_LIMITER_SWITCH, OUTPUT); 
	/* Motor Initialisation */

	/* X direction motor */
	MotorXa.motorInit();
	MotorXa.enableMotor();

	MotorXb.motorInit();
	MotorXb.enableMotor();

	MotorYa.motorInit(); 
	MotorYa.enableMotor(); 


	/* Initialisint sensor */
	while (m_Sensor_Xa.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR Xa"); 
		Serial.println("CHECK THE SENSORS"); 
		errorIndicator(); 
	}
	Serial.println("SUCCESSFULLY INITIALISED SENSOR Xa");

	/* Initialisint sensor */
	while (m_Sensor_Xb.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR Xb");
		Serial.println("CHECK THE SENSORS");
		errorIndicator(); 
	}
	Serial.println("SUCCESSFULLY INITIALISED SENSOR Xb");

	/* Initialisint sensor */
	while (m_Sensor_Ya.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR Ya");
		Serial.println("CHECK THE SENSORS");
		errorIndicator(); 
	}
	Serial.println("SUCCESSFULLY INITIALISED SENSOR Ya");

	/* Connecting modbus to client */
	while (myController.connectToClient()==false)
	{
		Serial.println("FAILED TO CONNECT TO MASTER"); 
		errorIndicator();
	}
	Serial.println("CONNECTED TO CLIENT");
	
	//setTimerInterrupt();

	Xa_switch.init(); /* initialising Xa limiter switch */

	setupLEDIndicators();

	PROG_INITIALISED = 110; 
}

/* ============================================================== */
/* LOOP -------------------------------------*/

noDelay fun1Time(1);
int CONTROL_KEY = 0; 

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
	myController.toClientData[2] = m_Sensor_Xa.getCount();
	myController.toClientData[3] = m_Sensor_Xb.getCount(); /*index 3 in labview*/
	myController.toClientData[4] = MotorYa.getState(); 
	myController.toClientData[5] = m_Sensor_Ya.getCount();

	myController.sendDataToClient(); 
	/* MOTOR CONTROLS */
	CONTROL_KEY = myController.getCtrlData(); 

	/* motor should'nt move if limiterSwitch is pressed and still the control is pressed down */

	if ((Xa_switch.getFlag() == 1) && (CONTROL_KEY == CTRL_DOWN)) {
		MotorXa.motorHALT();
		MotorXb.motorHALT();
		m_Sensor_Xa.resetSensor(); 
		m_Sensor_Xb.resetSensor(); 
	}

	switch (CONTROL_KEY) {

	case 0: 
		ledIndicator_OFF(IND_LED1);
		ledIndicator_OFF(IND_LED2);
		ledIndicator_OFF(IND_LED3);
		ledIndicator_OFF(IND_LED4);
		ledIndicator_OFF(IND_LED5);


		MotorXa.motorSTOP(); 
		MotorXb.motorSTOP(); 
		MotorYa.motorSTOP(); 

		break;

	case CTRL_UP:
		ledIndicator_ON(IND_LED1); 
		
		MotorYa.motorSTOP(); 

		if (fun1Time.update()) {	/* non blocking delay for soft start */
			MotorXa.ccRotate();
			MotorXb.cRotate();
		}

		//m_Sensor_Xa.updateData(1);	/* increment sensor count when motor goes forward */
		//m_Sensor_Xb.updateData(1); 

		break;

	case CTRL_DOWN:
		ledIndicator_OFF(IND_LED1);
		ledIndicator_ON(IND_LED2);

		MotorYa.motorSTOP(); 

		if (fun1Time.update()) {
			MotorXa.cRotate();
			MotorXb.ccRotate();
		}

		//m_Sensor_Xa.updateData(-1); 
		//m_Sensor_Xb.updateData(-1); 

		break;

	case CTRL_RIGHT: 
		ledIndicator_OFF(IND_LED1); 
		ledIndicator_OFF(IND_LED2); 
		ledIndicator_ON(IND_LED3); 

		MotorXa.motorSTOP(); 
		MotorXb.motorSTOP(); 


		if (fun1Time.update()) {
			MotorYa.cRotate();
		}
		//m_Sensor_Ya.updateData(1); 

		break; 

	case CTRL_LEFT: 
		ledIndicator_OFF(IND_LED1); 
		ledIndicator_OFF(IND_LED2); 
		ledIndicator_OFF(IND_LED3); 
		ledIndicator_ON(IND_LED4); 

		MotorXa.motorSTOP(); 
		MotorXb.motorSTOP();


		if (fun1Time.update()) {
			MotorYa.ccRotate();
		}
		//m_Sensor_Ya.updateData(-1); 

		break;

	case CTRL_SET_ORIGIN: 

		ledIndicator_OFF(IND_LED1);
		ledIndicator_OFF(IND_LED2);
		ledIndicator_OFF(IND_LED3);
		ledIndicator_OFF(IND_LED4);
		ledIndicator_ON(IND_LED5); 

		while (Xa_switch.getFlag() != 1) {
			ledIndicator_ON(IND_LED1); 
			ledIndicator_ON(IND_LED2);

			if (fun1Time.update()) {
				MotorXa.cRotate();
				MotorXb.ccRotate();
			}

			myController.poll(); 

		}

		MotorXa.motorHALT(); 
		MotorXb.motorHALT(); 
		m_Sensor_Xa.resetSensor();
		m_Sensor_Xb.resetSensor();
		break;

	case CTRL_CALIBRATE_BOUNDS:
		ledIndicator_OFF(IND_LED1);
		ledIndicator_OFF(IND_LED2);
		ledIndicator_OFF(IND_LED3);
		ledIndicator_OFF(IND_LED4);
		ledIndicator_OFF(IND_LED5);

		/* find the origin by ctrl_set_origin procedure
		* then move the Xmotor until it presses the x_switch
		* store the counter data
		* then move Ymotor until it presses the y_switch
		* store the counter data
		*/

		break;

	default: 
		ledIndicator_OFF(IND_LED1);
		ledIndicator_OFF(IND_LED2);
		ledIndicator_OFF(IND_LED3);
		ledIndicator_OFF(IND_LED4);
		ledIndicator_OFF(IND_LED5);
		
		MotorXa.motorSTOP(); 
		MotorXa.motorSTOP(); 
		MotorYa.motorSTOP(); 

		//MotorXa.motorHALT();
		//MotorXb.motorHALT();
	}

	/* Auto correcting motors if their count differ by 2 */
//int diff = m_Sensor_Xa.getCount() - m_Sensor_Xb.getCount(); 
//if (diff >= 2 || diff <= -2) {
//	/*
//	* if motorXa is ahead by 2 counts, 
//	* then HALT motorXa, ccRotate motorXb untill count of both 
//	* becomes equal 
//	*/
//	while (m_Sensor_Xa.getCount() > m_Sensor_Xb.getCount()) {
//		MotorXa.motorHALT();
//		MotorXb.ccRotate(); 
//		m_Sensor_Xa.updateData(1); 
//		m_Sensor_Xb.updateData(1); 
//	}
//	/* if motorXb is ahead by 2 counts, 
//	* then HALT motorXb, cRotate motorXa until count of both 
//	* becomes equal 
//	*/
//	while (m_Sensor_Xb.getCount() > m_Sensor_Xa.getCount()) {
//		MotorXb.motorHALT(); 
//		MotorXa.cRotate(); 
//		m_Sensor_Xa.updateData(1); 
//		m_Sensor_Xb.updateData(1); 
//	}
//}


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

	toggleLedIndicator(IND_LED6);
	/*-------------------------------------------------*/
}

/* ============================================================== */
/* EVENT ------------------------------------*/
void serialEvent1() {
	//statements
	myController.readFromClient();
}

/* ============================================================== */
/* ISR --------------------------------------*/
ISR(TIMER2_COMPA_vect) {
	//interrupt commands for TIMER 2 here
	/* checking switch status */
	Xa_switch.checkSwitch(); 

	/* updating counter data */
	//switch (CONTROL_KEY){
	//case CTRL_UP: 
	//	m_Sensor_Xa.updateData(1);	/* increment sensor count when motor goes forward */
	//	m_Sensor_Xb.updateData(1);
	//	break;
	//case CTRL_DOWN:
	//	m_Sensor_Xa.updateData(-1); 
	//	m_Sensor_Xb.updateData(-1); 
	//	break;
	//case CTRL_RIGHT:
	//	m_Sensor_Ya.updateData(1);
	//	break;
	//case CTRL_LEFT:
	//	m_Sensor_Ya.updateData(-1); 
	//	break;
	//}


}
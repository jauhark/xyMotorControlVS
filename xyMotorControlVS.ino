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

noDelay fun1Time(1); 
noDelay fun2Time(200); 
noDelay fun3Time(300); 
noDelay fun4Time(400); 
/* ============================================================== */
/* SETUP -----------------------------------*/

int PROG_INITIALISED = 0; 


/* state variables for motors*/
typedef struct x_direction {
	int pwmVal;
	int state;
};

x_direction x_motorCW;
x_direction x_motorCCW;


void setup() {
	/* Motor Initialisation */

	/* X direction motor */
	//MotorXa.motorInit();
	//MotorXa.enableMotor();

	//MotorXb.motorInit();
	//MotorXb.enableMotor();
	pinMode(MXa_C_pwm, OUTPUT); 

	x_motorCW.pwmVal = 0; 
	x_motorCW.state = M_SOFF; 

	x_motorCCW.pwmVal = 0;
	x_motorCCW.state = M_SOFF;

	/* Initialisint sensor */
	if (ir_Sensor_1.initSensor() == false) {
		Serial.println("FAILED TO INITIALISE SENSOR 1");
		Serial.println("CHECK THE SENSORS");
		while (1);
	}
	else Serial.println("SUCCESSFULLY INITIALISED SENSOR 1");

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

	PROG_INITIALISED = 110; 
}

/* ============================================================== */
/* LOOP -------------------------------------*/

#define MXa_C_pwm 12
#define MXa_C_en    13
#define MXa_CC_pwm  11
#define MXa_CC_en   10

#define MXb_C_pwm  8
#define MXb_C_en  9
#define MXb_CC_pwm  7
#define MXb_CC_en 6


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
	//myController.toClientData[0] = PROG_INITIALISED; 
	//myController.toClientData[1] = MotorXa.getState(); 
	//myController.toClientData[2] = ir_Sensor_1.getCount(); 
	//myController.toClientData[3] = MotorXb.getState(); 
	//myController.sendDataToClient(); 



	/* MOTOR DIRECTION CONTROL UPDATE */
	/*-------------------------------------------------*/
	/*  3 states for each motor movement 
	*	M_SOFF: motor steady off
	*	M_TON:	motor turnign ON. Not fully ON. PWM ramps
	*	M_SON:	motor Fully ON. PWM Steady limited value
	*/
	/* WHEN PRESSED FORWARD*/
	//if (myController.getCtrlData(0) > 0) {

	//	/* ROTATE CW */

	//	ledIndicator_ON(IND_LED3); 
	//	if (x_motorCW.state==M_TON) {
	//		/* if switch is ON & if motor is M_TON state, PWM ramps to limted value.
	//		* Then change state to M_SON
	//		*/
	//		x_motorCW.pwmVal += 1;
	//		if (x_motorCW.pwmVal > 250) {
	//			x_motorCW.pwmVal = 250;
	//			x_motorCW.state = M_SON;
	//		}
	//	}
	//	else if (x_motorCW.state == M_SON) {
	//		/* if switch is ON & if motor is M_SON state, 
	//		*	keep the motor running in pwm_limited value
	//		*/
	//	}
	//	else {
	//		/* if switch is ON & if motor is not in M_TON or M_SON state, 
	//		* ie, if motor is OFF, then set motor state as M_TON
	//		*/
	//		x_motorCW.state = M_TON; 
	//		x_motorCW.pwmVal = 0; 
	//	}
	//	/* updates the pwm value to motor */
	//	MotorXa.c_pwmValue = x_motorCW.pwmVal;
	//	MotorXa.cRotate(); 

	//	MotorXb.c_pwmValue = x_motorCW.pwmVal;
	//	MotorXb.cRotate(); 
	//}
	//else {
	//	/* if switch is not pressed, Stop motor and sets its state as M_SOFF */
	//	ledIndicator_OFF(IND_LED3); 
	//	x_motorCW.state = M_SOFF; 
	//	MotorXa.stop_cRotate(); 
	//	MotorXb.stop_cRotate();
	//}


	///* WHEN PRESSED REVERSE */
	//if (myController.getCtrlData(2) > 0) {
	//	/* ROTATE CCW */
	//	ledIndicator_ON(IND_LED4); 

	//	if (x_motorCCW.state == M_TON) {
	//		x_motorCCW.pwmVal += 1;
	//		if (x_motorCCW.pwmVal > 250) {
	//			x_motorCCW.pwmVal = 250;
	//			x_motorCCW.state = M_SON;
	//		}
	//	}
	//	else if (x_motorCCW.state == M_SON) {
	//		/* keep it as it is */
	//	}
	//	else {
	//		x_motorCCW.state = M_TON;
	//		x_motorCCW.pwmVal = 0;
	//	}

	//	MotorXa.cc_pwmValue = x_motorCCW.pwmVal; ;
	//	MotorXa.ccRotate(); 

	//	MotorXb.cc_pwmValue = x_motorCCW.pwmVal;
	//	MotorXb.ccRotate(); 
	//}
	//else {
	//	ledIndicator_OFF(IND_LED4); 

	//	x_motorCCW.state = M_SOFF;
	//	MotorXa.stop_ccRotate(); 
	//	MotorXb.stop_ccRotate(); 
	//}

	analogWrite(MXa_C_pwm, 100);
	delay(1);
	/*-------------------------------------------------*/
}

/* ============================================================== */
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
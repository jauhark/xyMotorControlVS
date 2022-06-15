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

typedef enum m_State {
	M_SOFF, M_SON, M_TON  //steadyOFF, steadyON, TurningON, turningOFF
};

/* state variables for motors*/
typedef struct x_direction {
	int pwmVal;
	int state;
};

x_direction x_motorCW;
x_direction x_motorCCW;

/* ----------------------------------------- */

void setup() {
	/* Motor Initialisation */

	/* X direction motor */
	MotorXa.motorInit();
	MotorXa.enableMotor();

	MotorXb.motorInit();
	MotorXb.enableMotor();
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

#define MXa_C_pwm 12
#define MXa_C_en    13
#define MXa_CC_pwm  11
#define MXa_CC_en   10

#define MXb_C_pwm  8
#define MXb_C_en  9
#define MXb_CC_pwm  7
#define MXb_CC_en 6


noDelay fun1Time(10);
unsigned long previousTime = 0;

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



	myController.toClientData[0] = 1; /*index 0 in labview*/
	myController.toClientData[1] = 1; 
	myController.toClientData[2] = 231; 
	myController.toClientData[3] = ir_Sensor_1.getCount(); /*index 3 in labview*/

	myController.sendDataToClient(); 

	/* MOTOR DIRECTION CONTROL UPDATE */
	/*-------------------------------------------------*/
	/*  3 states for each motor movement 
	*	M_SOFF: motor steady off
	*	M_TON:	motor turnign ON. Not fully ON. PWM ramps
	*	M_SON:	motor Fully ON. PWM Steady limited value
	*/
	/* WHEN PRESSED FORWARD*/
	if (myController.getCtrlData() ==1) {

		/* ROTATE CW */

		ledIndicator_ON(IND_LED3); 
		if (x_motorCW.state==M_TON) {
			/* if switch is ON & if motor is M_TON state, PWM ramps to limted value.
			* Then change state to M_SON
			*/
			if(fun1Time.update())
			x_motorCW.pwmVal += 1;
			if (x_motorCW.pwmVal > 250) {
				x_motorCW.pwmVal = 250;
				x_motorCW.state = M_SON;
			}
		}
		else if (x_motorCW.state == M_SON) {
			/* if switch is ON & if motor is M_SON state, 
			*	keep the motor running in pwm_limited value
			*/
		}
		else {
			/* if switch is ON & if motor is not in M_TON or M_SON state, 
			* ie, if motor is OFF, then set motor state as M_TON
			*/
			x_motorCW.state = M_TON; 
			x_motorCW.pwmVal = 0; 
		}
		/* updates the pwm value to motor */
		MotorXa.c_pwmValue = x_motorCW.pwmVal;
		MotorXa.cRotate(); 

		MotorXb.c_pwmValue = x_motorCW.pwmVal;
		MotorXb.cRotate(); 
	}
	else {
		/* if switch is not pressed, Stop motor and sets its state as M_SOFF */
		ledIndicator_OFF(IND_LED3); 
		x_motorCW.state = M_SOFF; 
		MotorXa.stop_cRotate(); 
		MotorXb.stop_cRotate();
	}


	/* WHEN PRESSED REVERSE */
	if (myController.getCtrlData()==3) {
		/* ROTATE CCW */
		ledIndicator_ON(IND_LED4); 

		if (x_motorCCW.state == M_TON) {
			if(fun1Time.update())x_motorCCW.pwmVal += 1;
			if (x_motorCCW.pwmVal > 250) {
				x_motorCCW.pwmVal = 250;
				x_motorCCW.state = M_SON;
			}
		}
		else if (x_motorCCW.state == M_SON) {
			/* keep it as it is */
		}
		else {
			x_motorCCW.state = M_TON;
			x_motorCCW.pwmVal = 0;
		}

		MotorXa.cc_pwmValue = x_motorCCW.pwmVal; ;
		MotorXa.ccRotate(); 

		MotorXb.cc_pwmValue = x_motorCCW.pwmVal;
		MotorXb.ccRotate(); 
	}
	else {
		ledIndicator_OFF(IND_LED4); 

		x_motorCCW.state = M_SOFF;
		MotorXa.stop_ccRotate(); 
		MotorXb.stop_ccRotate(); 
	}
	
	toggleLedIndicator(IND_LED8); 
	//delayMicroseconds(500); 
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
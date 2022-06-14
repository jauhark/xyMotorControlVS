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
	MotorXa.motorInit();
	MotorXa.enableMotor();

	MotorXb.motorInit();
	MotorXb.enableMotor();

	x_motorCW.pwmVal = 0; 
	x_motorCW.state = M_SOFF; 

	x_motorCCW.pwmVal = 0;
	x_motorCCW.state = M_SOFF;


	/* Connecting modbus to client */
	if (myController.connectToClient()) {
		Serial.println("FAILED TO CONNECT!");
		while (1);
	}
	else Serial.println("CONNECTED TO CLIENT");

	setupLEDIndicators(); 
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
	if (myController.getCtrlData(0) > 0) {

		/* ROTATE CW */

		ledIndicator_ON(IND_LED3); 

		if (x_motorCW.state==M_TON) {
			x_motorCW.pwmVal += 1;
			if (x_motorCW.pwmVal > 250) {
				x_motorCW.pwmVal = 250;
				x_motorCW.state = M_SON;
			}
		}
		else if (x_motorCW.state == M_SON) {
			/* keep it as it is */
		}
		else {
			x_motorCW.state = M_TON; 
			x_motorCW.pwmVal = 0; 
		}

		MotorXa.c_pwmValue = x_motorCW.pwmVal;
		MotorXa.cRotate(); 

		MotorXb.c_pwmValue = x_motorCW.pwmVal;
		MotorXb.cRotate(); 
	}
	else {
		ledIndicator_OFF(IND_LED3); 
		x_motorCW.state = M_SOFF; 
		MotorXa.stop_cRotate(); 
		MotorXb.stop_cRotate();
	}

	if (myController.getCtrlData(2) > 0) {
		/* ROTATE CCW */
		ledIndicator_ON(IND_LED4); 

		if (x_motorCCW.state == M_TON) {
			x_motorCCW.pwmVal += 1;
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
	delay(1); 
}

/* ============================================================== */
/* EVENT ------------------------------------*/

void serialEvent2() {
	//statements
	myController.readFromClient();
}
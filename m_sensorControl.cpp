// 
// 
// 

#include "m_sensorControl.h"

/*================================================================*/
/* constructor */
ir_Sensor::ir_Sensor(int pinNo) {
	pin = pinNo;
	memset(raw_SensorVal, 0, sizeof(raw_SensorVal));
	arrayMagnitude = 0;
	counter = 0;
	stateChangeDetected = 0;
}

/*------------------------------------------------------*/
/* initialisation */
bool ir_Sensor::initSensor() {
	pinMode(pin, INPUT_PULLUP); 
	arrayMagnitude = 0; 
	for (int i = 0; i < IR_BUFF_SIZE; i++){
	raw_SensorVal[i] = digitalRead(pin);
	Serial.println(raw_SensorVal[i]);
	if (raw_SensorVal[i] == IR_SENSOR_DETECTED)arrayMagnitude++;
	delay(10);
	}
	Serial.println(); 

	/* change state by checking buffer */
	if (arrayMagnitude > IR_NOISE_HIGH_THRESH) {	//taking sample of initial state and storing the state
		initialSensorState = true;	/* IR SENSOR DETECTED STATE */
		//SensorState = initialSensorState; 
		return true;	/* returns true when detected or not detected value */
	}
	else if (arrayMagnitude < IR_NOISE_LOW_THRESH) {
		initialSensorState = false; /* IR SENSOR NOT DETECTED STATE */
		//SensorState = initialSensorState; 
		return true;
	}
	else return false;	/* returns false when high noise */
}

/*------------------------------------------------------*/
/* read and update buffer and checks magnitude and update state */
void ir_Sensor::updateData(int inc) {

	arrayMagnitude = 0; 
	for (int i = 0; i < (IR_BUFF_SIZE - 1); i++) {
		raw_SensorVal[i] = raw_SensorVal[i + 1];
		/* if sensor moves from initial state */
		if (raw_SensorVal[i] != initialSensorState)arrayMagnitude++; 
	}
	raw_SensorVal[IR_BUFF_SIZE - 1] = digitalRead(pin); 

	/* change state by checking buffer */
	/* if sensor reading current state isn't initial state, then increment counter */
	if (arrayMagnitude > IR_NOISE_HIGH_THRESH) {	//taking sample of initial state and storing the state
		if (SensorState == initialSensorState) {
			SensorState = !(initialSensorState);
			counter += inc; 
		}
	}
	else if (arrayMagnitude < IR_NOISE_LOW_THRESH) {
		SensorState = initialSensorState;
	}

	//if (counter > IR_COUNTER_LIMIT) { counter = IR_COUNTER_LIMIT; }
	//if (counter < IR_COUNTER_LIMIT) { counter = IR_COUNTER_LIMIT; }
}

//void ir_Sensor::updateData(int inc) {
//	static int switch_pressed = 0; 
//	static int switch_notPressed = 0;
//
//	if (digitalRead(pin) != initialSensorState) {
//		switch_notPressed = 0; 
//		switch_pressed++; 
//		if (switch_pressed > ) {
//			switch_pressed = 100;
//			counter += inc; 
//		}
//	}
//	else {
//		switch_pressed = 0; 
//	}
//
//}
/*------------------------------------------------------*/
/* resets sensor datas */

void ir_Sensor::resetSensor() {
	memset(raw_SensorVal, 0, sizeof(raw_SensorVal));
	arrayMagnitude = 0;
	counter = 0;
	stateChangeDetected = 0;
	//initSensor(); 
}





/*================================================================*/
ir_Sensor m_Sensor_Xa(SENSOR1_PIN); 
ir_Sensor m_Sensor_Xb(SENSOR2_PIN); 
ir_Sensor m_Sensor_Ya(SENSORY_PIN);
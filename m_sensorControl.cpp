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

	resetSensor(); 

	arrayMagnitude = 0; 
	for (int i = 0; i < IR_BUFF_SIZE; i++){
	raw_SensorVal[i] = digitalRead(pin);
	Serial.println(raw_SensorVal[i]);
	if (raw_SensorVal[i] == IR_SENSOR_DETECTED)arrayMagnitude++;
	delay(100);
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
void ir_Sensor::updateData() {
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
			counter++;
		}
	}
	else if (arrayMagnitude < IR_NOISE_LOW_THRESH) {
		SensorState = initialSensorState;
	}

	if (counter > IR_COUNTER_LIMIT)counter = 0; 

}

/*------------------------------------------------------*/
/* resets sensor datas */

void ir_Sensor::resetSensor() {
	memset(raw_SensorVal, 0, sizeof(raw_SensorVal));
	arrayMagnitude = 0;
	counter = 0;
	stateChangeDetected = 0;
}





/*================================================================*/
ir_Sensor ir_Sensor_1(SENSOR1_PIN); 
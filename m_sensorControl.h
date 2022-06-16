// m_sensorControl.h

#ifndef _M_SENSORCONTROL_h
#define _M_SENSORCONTROL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*================================================================*/
/* SENSOR PIN DEFINITIONS */	
#define	SENSOR1_PIN 23
#define SENSOR2_PIN 25

#define IR_COUNTER_LIMIT 1000
#define X_MAX 1000
#define X_ORIGIN 0

/*-----------------------------------------------*/
/* PROGRAM FOR SENSOR CONTROL */

#define NO_OF_SENSORS 2
#define IR_BUFF_SIZE 11

#define IR_SENSOR_DETECTED LOW
#define IR_SENSOR_NOT_DETECTED HIGH

#define IR_NOISE_HIGH_THRESH (IR_BUFF_SIZE-2)
#define IR_NOISE_LOW_THRESH 2


/* sensor 
* contains pin and value
* each sensor value has to be buffered and taken to eliminate noise
* sensor value analysed and count increment for each sensor
*/

/*
* class ir_Sensor: 
* @Constructor_Params: IR_SENSOR_PIN
* @public_funs: 
*	initSensor()	- Initialises the sensor. Takes buffer full of readings
*					- and analyses buffer to find starting sensor value.  
*					- if high noise, false is returned. 
*	updateData()	- called every loop. It left shifts all older readings 
*					- and add new reading to buffer. It also calculates 
*					- buffer magnitude, which is used to decide current sensor
*					- value and eliminate noise. 
*	getCount()		- returns the current count value 
*	resetSensor()	- resets all sensor values. counter and buffer and calls init
*	
*/

class ir_Sensor {
private: 
	int pin; 
	int raw_SensorVal[IR_BUFF_SIZE];	/* Taken every 1 ms */
	bool initialSensorState;	/* true-> detected, false-> not detected */
	int stateChangeDetected; 
	bool SensorState;	/* set after analysing buffer */
	int arrayMagnitude; /* condition for set/reset SensorState */
	int counter;		/* counter data */
	bool counterIncFlag; /* counter Increment Flag */
public:	
	ir_Sensor(int); 
	bool initSensor(); 
	void updateData(int );	/* reads to buffer, checks magnitude, update state */
	int getCount() {
		return counter; 
	}
	void resetSensor(); 
	void sensorSpeed(); 									
};
/*================================================================*/
extern ir_Sensor ir_Sensor_1;


#endif


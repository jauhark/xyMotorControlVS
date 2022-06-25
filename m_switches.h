// m_switches.h

#ifndef _M_SWITCHES_h
#define _M_SWITCHES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*==================================================*/
/*
* define pins for limiter switches here.
*/
#define Xa_limSwitchPin 25
/*
* pin definition for limiter switch 2 here
* pin definition for limiter switch 3 here. 
* ... 
*/

/* ============================================================== */
/*
* create objects for every limiter switches. 
* for avoiding noise effects. 
* 
* @Constructor: switchInput(PINNUMBER) creates switch object on given pin
* 
* @init() initialises the given pinnumber as input
* 
* @checkSwitch() should be called on timer for analysing consecutive readings and 
* decide switch position
* 
* @getFlag() returns the current pin Status. 1= pressed, (active LOW), 0= not pressed (HIGH CONDITION)
* 
*/
class switchInput {
private: 
	uint8_t pin;
	uint16_t countHigh;
	uint16_t countLow;
	uint8_t Flag;
public: 
	switchInput(int _pin) {
		pin = _pin;
		countHigh = 0;
		countLow = 0;
		Flag = 0;	//low
	}

	void init() {
		pinMode(pin, INPUT_PULLUP);
	}

	void checkSwitch() {
		if (digitalRead(pin) == HIGH) {
			countLow = 0; 
			countHigh++; 
			if (countHigh > 5) {
				countHigh = 5;
				Flag = 0; 
			}
		}
		else {
			countHigh = 0; 
			countLow++; 
			if (countLow > 5) {
				countLow = 5; 
				Flag = 1;	/* Flag set on Active LOW*/
			}
		}
	}

	uint8_t getFlag() {
		return Flag;
	}
};

/* ============================================================== */
/* creating switch objects for all limiter switches */
switchInput Xa_switch(Xa_limSwitchPin);
/*
* create object for limiter switch 2
* create object for limtier switch 3
*/

/*==================================================*/


#endif


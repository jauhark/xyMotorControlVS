// m_switches.h

#ifndef _M_SWITCHES_h
#define _M_SWITCHES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*==================================================*/
#define Xa_limSwitchPin 25

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
			countHigh++; 
			if (countHigh > 5) {
				countHigh = 5;
				Flag = 0; 
				countLow = 0; 
			}
		}
		else {
			countLow++; 
			if (countLow > 5) {
				countLow = 5; 
				Flag = 1;	/* Flag set on Active LOW*/
				countHigh = 0; 
			}
		}
	}

	uint8_t getFlag() {
		return Flag;
	}
};

/* creating switch objects for all limiter switches */
switchInput Xa_switch(Xa_limSwitchPin);

/*==================================================*/


#endif


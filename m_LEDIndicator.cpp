// 
// 
// 

#include "m_LEDIndicator.h"



uint8_t ledIndicatorArray[] = {
	IND_LED1, 
	IND_LED2, 
	IND_LED3, 
	IND_LED4, 
	IND_LED5, 
	IND_LED6, 
	IND_LED7,
	IND_LED8
};

void setupLEDIndicators() {
	int NO_OF_INDICATORS=sizeof(ledIndicatorArray);

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		pinMode(ledIndicatorArray[i], OUTPUT);
	}

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], LOW);
	}
	delay(DELAY_LED); 

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], HIGH);
	}
	delay(DELAY_LED);

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], LOW);
	}
	delay(DELAY_LED);

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], HIGH);
		delay(DELAY_LED_2);
	}

	delay(DELAY_LED_2);

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], LOW);
		delay(DELAY_LED_2);
	}

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], HIGH);
	}
	delay(DELAY_LED);

	for (int i = 0; i < NO_OF_INDICATORS; i++) {
		digitalWrite(ledIndicatorArray[i], LOW);
	}
}


void ledIndicator_ON(uint8_t pinNo) {
	digitalWrite(pinNo, HIGH); 
}

void ledIndicator_OFF(uint8_t pinNo) {
	digitalWrite(pinNo, LOW); 
}

void blinkLedIndicator(uint8_t pinNo) {
	digitalWrite(pinNo, HIGH); 
	delay(1); 
	digitalWrite(pinNo, LOW); 
}

void toggleLedIndicator(uint8_t pinNo) {
	digitalWrite(pinNo, !(digitalRead(pinNo))); 
}
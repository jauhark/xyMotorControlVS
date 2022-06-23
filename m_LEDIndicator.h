// m_LEDIndicator.h

#ifndef _M_LEDINDICATOR_h
#define _M_LEDINDICATOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*========================================================*/
#define DELAY_LED 200
#define DELAY_LED_2 100

#define IND_LED1 A8
#define IND_LED2 A9
#define IND_LED3 A10
#define IND_LED4 A11
#define IND_LED5 A12
#define IND_LED6 A13
#define IND_LED7 A14
#define IND_LED8 A15

/*-------------------------------------------------------------*/
/* ADD LED INDICATORS HERE ------------------------------------*/
#define motorCW_LED IND_LED5
#define motorCCW_LED IND_LED6
#define timerInterrupt_LED IND_LED1
/*-------------------------------------------------------------*/

void setupLEDIndicators(); 
void ledIndicator_ON(uint8_t); 
void ledIndicator_OFF(uint8_t);
void blinkLedIndicator(uint8_t);
void toggleLedIndicator(uint8_t); 
void errorIndicator(); 
/*========================================================*/

#endif


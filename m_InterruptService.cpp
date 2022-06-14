// 
// 
// 

#include "m_InterruptService.h"

int timer100mS = 0;

void setTimerInterrupt() {
	/*
	Setting timer interrrupt for reading data every 1ms
	1 total reading will be in 10mS
	->timer interrupt 1000hz
	*/
	cli(); /*clear all interrupts*/
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 249;
	TCCR1A |= (1 << WGM01); /* turn on ctc mode */
	TCCR1B |= (1 << CS11) | (1 << CS10);  /*Set CS10 and CS10 bits for 64 prescaler*/
	TIMSK1 |= (1 << OCIE1A); /*enable timer compare interrupt*/

	sei();

}
// 
// 
// 

#include "m_InterruptService.h"

/* 10mS timer */

void setTimerInterrupt() {
	noInterrupts();
	// Clear registers
	TCCR2A = 0;
	TCCR2B = 0;
	TCNT2 = 0;

	// 100.16025641025641 Hz (16000000/((155+1)*1024))
	OCR2A = 155;
	// CTC
	TCCR2A |= (1 << WGM21);
	// Prescaler 1024
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	// Output Compare Match A Interrupt Enable
	TIMSK2 |= (1 << OCIE2A);
	interrupts();
}
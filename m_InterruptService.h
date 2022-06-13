// m_InterruptService.h

#ifndef _M_INTERRUPTSERVICE_h
#define _M_INTERRUPTSERVICE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*==================================================*/
extern int timer100mS; 

void setTimerInterrupt();

/*==================================================*/

#endif


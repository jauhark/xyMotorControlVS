// m_MotorDrive.h

#ifndef _M_MOTORDRIVE_h
#define _M_MOTORDRIVE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*
* PROGRAM CODE FOR MOTOR DRIVE
*/
/*================================================================*/
/* MOTOR inits-----------------------------*/

#define C_pwm  8
#define C_en  9
#define CC_pwm  7
#define CC_en 6

#define T_STATE_RESOLUTION 100
#define T_STATE_PWM_INIT_VALUE 24
#define T_STATE_PWM_INCREMENT 10
#define PWM_LIMIT 1020


typedef enum m_State{
    M_SOFF, M_SON, M_TON, M_TOFF  //steadyOFF, steadyON, TurningON, turningOFF
}m_State;
/*
   control single motor
*/
/*----------------------------------------*/
class motorObject
{
private:

	int motorON;  /* external control input */

    int cEnablePin; /* clockwise enable pin */
    int ccEnablePin; /* counter clockwise enable Pin */
    int cPwmPin; /* clockwise Pwm pin */
    int ccPwmPin; /* cc pwm pin */
    int pwmValue;   /* Motor running pwm Value */

    m_State motorState; /*0-off, 1-on, 2-turningOn, 3-turningOFF */

    int T_State_count; 
    int T_State_Number; /* resolution of transcient state */
    int T_State_pwm_Increment; /* increment value of pwm */

    void _motorRotate(int); /* analogwrite given pin */
    void _motorStop(int);    /* analogWrite given pin */
    void _setState(m_State _state) {
        motorState = _state; 
    };  /*0-OFF, 1-ON, 2-turningON, 3-turningOFF */

public:

    motorObject(int, int, int, int);/*cEN, ccEN, cPwm, ccPwm*/

    void motorInit();   /* Motor Initialisation */

    void enableMotor() {    /* Motor ENABLE */
        digitalWrite(cEnablePin, HIGH); 
        digitalWrite(ccEnablePin, HIGH);
    }

    void disableMotor() {   /* Motor DISABLE */
        digitalWrite(cEnablePin, LOW);
        digitalWrite(ccEnablePin, LOW);
    }

    void cRotate(); /* Rotate CW */
    void stop_cRotate();    /* stop CW */
    void ccRotate();    /* Rotate CCW */
    void stop_ccRotate();   /* stop CCW */
    void motorHALT();   /* stop motor */

    m_State getState() {
        return motorState; 
    }


    void updateTranscientPWM(); 

        
};
/*----------------------------------------*/
/*================================================================*/
extern motorObject Motor1;

#endif


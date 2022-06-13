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

#define MXa_C_pwm 12
#define MXa_C_en    13
#define MXa_CC_pwm  11
#define MXa_CC_en   10

#define MXb_C_pwm  8
#define MXb_C_en  9
#define MXb_CC_pwm  7
#define MXb_CC_en 6

#define T_STATE_RESOLUTION 100
#define T_STATE_PWM_INIT_VALUE 24
#define T_STATE_PWM_INCREMENT 1
#define PWM_LIMIT 255


typedef enum m_State{
    M_SOFF, M_SON, M_TON, M_TOFF  //steadyOFF, steadyON, TurningON, turningOFF
};
/*
   control single motor
*/
/*----------------------------------------*/
class motorObject
{
private:

	int motorON;  /* external control input */
    bool motorACTIVEState; /*MOTOR CURRENTLY IN ACTIVE */

    int cEnablePin; /* clockwise enable pin */
    int ccEnablePin; /* counter clockwise enable Pin */
    int cPwmPin; /* clockwise Pwm pin */
    int ccPwmPin; /* cc pwm pin */
    int T_State_pwm_Increment; /* increment value of pwm */


    int c_pwmValue;   /* Motor running pwm Value */
    int cc_pwmValue;   /* Motor running pwm Value */

    void _motorRotate(int, int); /* analogwrite given pin */
    void _motorStop(int);    /* analogWrite given pin */

    m_State cw_motorState; /*0-off, 1-on, 2-turningOn, 3-turningOFF */
    m_State ccw_motorState;

    void cw_setState(m_State _state) { cw_motorState = _state; }
    m_State cw_getState() { return cw_motorState; }

    void ccw_setState(m_State _state) { ccw_motorState = _state; }
    m_State ccw_getState() { return ccw_motorState; }

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

    bool isMotorActive() {
        return motorACTIVEState;
    }
    void updateTranscientPWM(); 

        
};
/*----------------------------------------*/
/*================================================================*/
extern motorObject MotorXb;
extern motorObject MotorXa;


#endif


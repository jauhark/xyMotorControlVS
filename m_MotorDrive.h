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
#define PWM_LIMIT 250

<<<<<<< HEAD
typedef enum motorState {
    M_OFF, M_CW, M_TCW, M_CCW, M_TCCW   /*motor off, motorCW, motorTurningOnInCW,..,..*/
}motorState;
=======
typedef enum m_State{
    M_SOFF, M_SON, M_TON, M_TOFF  //steadyOFF, steadyON, TurningON, turningOFF
};
>>>>>>> parent of fccc812 (working fine)
/*
   control single motor
*/
/*----------------------------------------*/
class motorObject
{
private:
    int motorState; 
    int cEnablePin; /* clockwise enable pin */
    int ccEnablePin; /* counter clockwise enable Pin */
    int cPwmPin; /* clockwise Pwm pin */
    int ccPwmPin; /* cc pwm pin */
    motorState state; 

    void _motorRotate(int, int); /* analogwrite given pin */
    void _motorStop(int);    /* analogWrite given pin */

public:
    int T_State_pwm_Increment; /* increment value of pwm */

    int c_pwmValue;   /* Motor running pwm Value */
    int cc_pwmValue;   /* Motor running pwm Value */

    motorObject(int, int, int, int);/*cEN, ccEN, cPwm, ccPwm*/

    void motorInit();   /* Motor Initialisation */

    void enableMotor() {    /* Motor ENABLE */
        pinMode(ccEnablePin, OUTPUT); 
        pinMode(cEnablePin, OUTPUT); 
        pinMode(ccPwmPin, OUTPUT); 
        pinMode(cPwmPin, OUTPUT); 
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
    int getState() {
        return motorState; 
    }

        
};
/*----------------------------------------*/
/*================================================================*/
extern motorObject MotorXb;
extern motorObject MotorXa;


#endif


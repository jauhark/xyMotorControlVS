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

typedef enum motorState {
    /*
    * M_OFF: Motor is OFF State
    * M_ONCW: Motor in ON in CW direction
    * M_ONCCW: Motor is ON in CCW direction
    * M_TONCW: Motor is turning ON in CW direction
    * M_TONCCW: Motor is turning ON in CCW direction
    */
    M_OFF, M_ONCW, M_ONCCW, M_TONCW, M_TONCCW 
}motorState;

/*
* class motorObject: 
* @Constructor_Params: CW_EN, CCW_EN, CW_PWM_PIN, CCW_PWM_PIN
* @public_funs: 
*   motorInit()     - Initialises motor by setting the pins
*   enableMotor()   - Enables the motor EN pins
*   disableMotor()  - disables the motor EN pins
*   cRotate()       - rotates motor Clock wise, with Soft start
*                   - to be used only with if else ladder
*   ccRotate()      - rotates motor Counter Clock wise, with soft start
*                   - to be used only with if else ladder
*   motorHalt()     - Stop the motor
*   setState(param) - sets the motorstate 
*   getState()      - returns the motorState
*       
*/
/*----------------------------------------*/
class motorObject
{
private:
    int cEnablePin; /* clockwise enable pin */
    int ccEnablePin; /* counter clockwise enable Pin */
    int cPwmPin; /* clockwise Pwm pin */
    int ccPwmPin; /* cc pwm pin */

    motorState state; 

    void _motorRotate(int, int); /* analogwrite given pin */
    void _motorStop(int);    /* analogWrite given pin */

    void stop_ccRotate();   /* stop CCW */
    void stop_cRotate();    /* stop CW */

    int T_State_pwm_Increment; /* increment value of pwm */
    int pwmValue;

public:

    motorObject(int, int, int, int);/*cEN, ccEN, cPwm, ccPwm*/

    void motorInit();   /* Motor Initialisation */
    void enableMotor();   /* Motor ENABLE */
    void disableMotor() {   /* Motor DISABLE */
        digitalWrite(cEnablePin, LOW);
        digitalWrite(ccEnablePin, LOW);
    }
    void cRotate(); /* Rotate CW */
    void ccRotate();    /* Rotate CCW */
    void motorHALT();   /* stop motor */
    void setState(motorState _state) {
        state = _state;
    }
    int getState() {
        return state;
    }

        
};
/*----------------------------------------*/
/*================================================================*/
extern motorObject MotorXb;
extern motorObject MotorXa;


#endif


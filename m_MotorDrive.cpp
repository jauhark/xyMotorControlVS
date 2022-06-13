// 
// 
// 

#include "m_MotorDrive.h"

/*----------------------------------------*/
/*constructor*/
motorObject::motorObject(int _cEnablePin, int _ccEnablePin, int _cPwmPin,
    int _ccPwmPin)
{
    cEnablePin = _cEnablePin;
    ccEnablePin = _ccEnablePin;
    cPwmPin = _cPwmPin;
    ccPwmPin = _ccPwmPin;
    pwmValue = 0;
    motorON = 0;
    T_State_Number = T_STATE_RESOLUTION; 
    T_State_pwm_Increment = T_STATE_PWM_INCREMENT; 
    _setState(M_SOFF);  //steady off

}
/*----------------------------------------*/
/* Init */
void motorObject::motorInit() {
    _setState(M_SOFF); //steady off
    pinMode(cEnablePin, OUTPUT);
    pinMode(ccEnablePin, OUTPUT);
    pinMode(cPwmPin, OUTPUT);
    pinMode(ccPwmPin, OUTPUT);
    motorHALT();
}
/*----------------------------------------*/
/* rotate cw */
void motorObject::cRotate() {
    _motorRotate(cPwmPin); 
}
/*----------------------------------------*/
/* stop cw */
void motorObject::stop_cRotate() {
    _motorStop(cPwmPin); 
}
/*----------------------------------------*/
/* rotate ccw */
void motorObject::ccRotate() {
    _motorRotate(ccPwmPin); 
}
/*----------------------------------------*/
/* stop ccw */
void motorObject::stop_ccRotate() {
    _motorStop(ccPwmPin); 
}
/*----------------------------------------*/
/* Sudden stop motor */
void motorObject::motorHALT() {
    analogWrite(cPwmPin, 0); 
    analogWrite(ccPwmPin, 0); 
}

/*----------------------------------------*/
/* motor rotate */
/* TODO: Slow start */
void motorObject::_motorRotate(int pin) {
    _setState(M_TON); 
    pwmValue = 1000; 
    analogWrite(pin, pwmValue); 
}

/*----------------------------------------*/
/* motor turning off */
/* TODO: Slow stop */
void motorObject::_motorStop(int pin) {
    _setState(M_TOFF);
    pwmValue = 0; 
    analogWrite(pin, pwmValue);
}

/*----------------------------------------*/
/* transcient state pwm adjust (from timer interrupt) */
/* for soft start */
void motorObject::updateTranscientPWM() {

    /*
    * 4 states(T_State, S_State):
    * 00    (OFF Steady)
    * 01    (ON Steady)
    * 10    (turning ON)
    * 11    (turning OFF)
    */

    if (motorState==M_SOFF) {        /* OFF Steady */
    }
    else if (motorState==M_SON) {   /* ON Steady */
    }
    else if (motorState==M_TON) {   /* turning ON */
    }
    else if (motorState==M_TOFF) {   
    }

}

/*-------------------------------------------*/

/*---------------------------------------------------------------------*/

motorObject Motor1(C_en, CC_en, C_pwm, CC_pwm);

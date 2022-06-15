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
    c_pwmValue = 0;
    cc_pwmValue = 0; 
    T_State_pwm_Increment = T_STATE_PWM_INCREMENT; 
}
/*----------------------------------------*/
/* Init */
void motorObject::motorInit() {
    pinMode(cEnablePin, OUTPUT);
    pinMode(ccEnablePin, OUTPUT);
    pinMode(cPwmPin, OUTPUT);
    pinMode(ccPwmPin, OUTPUT);
    motorHALT();
}
/*----------------------------------------*/
/* rotate cw */
void motorObject::cRotate() {
    enableMotor(); 
    stop_ccRotate(); 
    _motorRotate(cPwmPin, c_pwmValue); 
}
/*----------------------------------------*/
/* stop cw */
void motorObject::stop_cRotate() { 
    c_pwmValue = 0; 
    _motorStop(cPwmPin); 
}
/*----------------------------------------*/
/* rotate ccw */
void motorObject::ccRotate() {
    enableMotor(); 
    stop_cRotate(); 
    _motorRotate(ccPwmPin, cc_pwmValue); 
}
/*----------------------------------------*/
/* stop ccw */
void motorObject::stop_ccRotate() {
    cc_pwmValue = 0;
    _motorStop(ccPwmPin); 
}
/*----------------------------------------*/
/* motor rotate */
/* TODO: Slow start */
void motorObject::_motorRotate(int pin, int val) {
    analogWrite(pin, val); 
}

/*----------------------------------------*/
/* motor turning off */
/* TODO: Slow stop */
void motorObject::_motorStop(int pin) {
    analogWrite(pin, 0);
}

/*----------------------------------------*/
/* Sudden stop motor */
void motorObject::motorHALT() {
    c_pwmValue = 0; 
    cc_pwmValue = 0; 
    analogWrite(cPwmPin, 0);
    analogWrite(ccPwmPin, 0);
}


/*-------------------------------------------*/

/*---------------------------------------------------------------------*/

motorObject MotorXb(MXb_C_en, MXb_CC_en, MXb_C_pwm, MXb_CC_pwm);
motorObject MotorXa(MXa_C_en, MXa_CC_en, MXa_C_pwm, MXa_CC_pwm); 
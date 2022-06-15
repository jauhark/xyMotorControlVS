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
    T_State_pwm_Increment = T_STATE_PWM_INCREMENT; 

    pwmValue = 0; 
    state = M_OFF; 
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
    if (state == M_TONCW) {
        pwmValue += 1; 
        if (pwmValue > PWM_LIMIT) {
            pwmValue = PWM_LIMIT; 
            state = M_ONCW; 
        }
    }
    else if (state == M_ONCW) {

    }
    else {
        state = M_TONCW;
        pwmValue = 0;
    }

    enableMotor();
    stop_ccRotate();
    _motorRotate(cPwmPin, pwmValue);
}


///*----------------------------------------*/
///* rotate cw */
//void motorObject::cRotate() {
//    enableMotor(); 
//    stop_ccRotate(); 
//    _motorRotate(cPwmPin, pwmValue); 
//}

/*----------------------------------------*/
/* stop cw */
void motorObject::stop_cRotate() { 
    _motorStop(cPwmPin); 
}


/*----------------------------------------*/
/* rotate ccw */
void motorObject::ccRotate() {

    if (state == M_TONCCW) {
        pwmValue += 1;
        if (pwmValue > PWM_LIMIT) {
            pwmValue = PWM_LIMIT;
            state = M_ONCCW;
        }
    }
    else if (state == M_ONCCW) {

    }
    else {
        state = M_TONCCW;
        pwmValue = 0;
    }

    enableMotor(); 
    stop_cRotate(); 
    _motorRotate(ccPwmPin, pwmValue); 
}

///*----------------------------------------*/
///* rotate ccw */
//void motorObject::ccRotate() {
//    enableMotor();
//    stop_cRotate();
//    _motorRotate(ccPwmPin, pwmValue);
//}

/*----------------------------------------*/
/* stop ccw */
void motorObject::stop_ccRotate() {
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
    pwmValue = 0; 
    state = M_OFF;
    analogWrite(cPwmPin, 0);
    analogWrite(ccPwmPin, 0);
}


/*-------------------------------------------*/

/*---------------------------------------------------------------------*/

motorObject MotorXb(MXb_C_en, MXb_CC_en, MXb_C_pwm, MXb_CC_pwm);
motorObject MotorXa(MXa_C_en, MXa_CC_en, MXa_C_pwm, MXa_CC_pwm); 
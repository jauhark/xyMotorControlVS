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
    motorON = 0;
    T_State_pwm_Increment = T_STATE_PWM_INCREMENT; 
    cw_setState(M_SOFF); 
    ccw_setState(M_SOFF); 

}
/*----------------------------------------*/
/* Init */
void motorObject::motorInit() {
    cw_setState(M_SOFF);
    ccw_setState(M_SOFF);
    pinMode(cEnablePin, OUTPUT);
    pinMode(ccEnablePin, OUTPUT);
    pinMode(cPwmPin, OUTPUT);
    pinMode(ccPwmPin, OUTPUT);
    motorHALT();
}
/*----------------------------------------*/
/* rotate cw */
void motorObject::cRotate() {
    /*
    * if motor is turning ON while switch is pressed, condition ONE
    * else if motor is Steady ON while switch is pressed, condition TWO
    * else motor is turning OFF or Steady OFF, condition three
    */
    Serial.println(); 
    Serial.print("Motor CW State: ");
    if (cw_getState() == M_TON) {
        Serial.println("TURNING ON"); 
        c_pwmValue += T_State_pwm_Increment;

        if (c_pwmValue > PWM_LIMIT) {
            c_pwmValue = PWM_LIMIT;
            cw_setState(M_SON); 
        }
    }
    else if (cw_getState() == M_SON) {
        Serial.println("STEADY ON");
        c_pwmValue =PWM_LIMIT;
    }
    else {
        Serial.println("JUST TURNING ON"); 
        cw_setState(M_TON); 
        c_pwmValue = 0;
    }
    Serial.println(); 
    Serial.print("PWM VALUE: "); 
    Serial.println(c_pwmValue); 

    _motorRotate(cPwmPin, c_pwmValue); 

    stop_ccRotate(); 
}
/*----------------------------------------*/
/* stop cw */
void motorObject::stop_cRotate() { 
/*    T_State_count = 0; 
    pwmValue = 0;*/ 
    c_pwmValue = 0;
    cw_setState(M_SOFF); 
    _motorStop(cPwmPin); 
}
/*----------------------------------------*/
/* rotate ccw */
void motorObject::ccRotate() {
    //Serial.println();
    //Serial.print("Motor CCW State: ");
    if (ccw_getState() == M_TON) {
        //Serial.println("TURNING ON");
        cc_pwmValue += T_State_pwm_Increment;

        if (cc_pwmValue > PWM_LIMIT) {
            cc_pwmValue = PWM_LIMIT;
            ccw_setState(M_SON);
        }
    }
    else if (ccw_getState() == M_SON) {
        //Serial.println("STEADY ON"); 
        cc_pwmValue = PWM_LIMIT;
    }
    else {
        //Serial.println("JUST TURNING ON"); 
        ccw_setState(M_TON);
        cc_pwmValue = 0;
    }
    Serial.println(); 
    //Serial.print("PWM VALUE"); 
    //Serial.println(cc_pwmValue); 

    _motorRotate(ccPwmPin, cc_pwmValue); 

    stop_cRotate(); 
}
/*----------------------------------------*/
/* stop ccw */
void motorObject::stop_ccRotate() {
    cc_pwmValue = 0;
    ccw_setState(M_SOFF); 
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
    analogWrite(cPwmPin, 0);
    analogWrite(ccPwmPin, 0);
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

}

/*-------------------------------------------*/

/*---------------------------------------------------------------------*/

motorObject MotorXb(MXb_C_en, MXb_CC_en, MXb_C_pwm, MXb_CC_pwm);
motorObject MotorXa(MXa_C_en, MXa_CC_en, MXa_C_pwm, MXa_CC_pwm); 
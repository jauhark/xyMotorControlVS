// 
// 
// 

#include "m_modBusComm.h"

/*================================================================*/

/*
   Constructor class
   @params: slaveId, BaudRate, noOfHoldingRegsRequired
*/
myClass::myClass(int ID = 1, int baud_Rate = BAUDRATE, int no_of_HR = 4) {

    slaveID = ID;
    baudRate = baud_Rate;
    noOfHR = no_of_HR;

	posFeedback_X = 6;
    posFeedback_Y = 2;

    ctrlData[0] = 0;
    ctrlData[1] = 0;
    ctrlData[2] = 0;
    ctrlData[3] = 0;

}

/*
    Connect to client
*/

int myClass::connectToClient() {
    if (!ModbusRTUServer.begin(slaveID, baudRate)) {
        return 1; //failed to connect
    }
    // configure holding register address
    ModbusRTUServer.configureHoldingRegisters(HRADD, 6);

    return 0; //connected
}

/*
  Accepts data from Client to local registers
*/
void myClass::readFromClient() {
    for (int i = 0; i < noOfHR; i++) {
        ctrlData[i] = ModbusRTUServer.holdingRegisterRead(HRADD + i);
        //Serial.print("HR ");
        //Serial.print(i);
        //Serial.print(": ");
        //Serial.println(ctrlData[i]);
    }
    Serial.println();
}



/*================================================================*/
myClass myController(SLAVE_ID, BAUDRATE, HRNUM);
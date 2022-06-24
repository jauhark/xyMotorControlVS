// 
// 
// 

#include "m_modBusComm.h"

/*================================================================*/

/*
   Constructor class
   @params: slaveId, BaudRate, noOfHoldingRegsRequired
*/
myClass::myClass(int ID = 1, int baud_Rate=9600, int no_of_HR=8) {
    slaveID = ID;
    baudRate = baud_Rate;
    noOfHR = no_of_HR;
    ctrlValue = 0; 
    ctrlData = 0;
    no_ClientData = NUM_CLIENT_DATA; 
}

/*
    Connect to client
*/

bool myClass::connectToClient() {
    if (!ModbusRTUServer.begin(slaveID, baudRate)) {
        return false; //failed to connect
    }
    // configure holding register address
    ModbusRTUServer.configureHoldingRegisters(HRADD, noOfHR);

    return true; //connected
}

/*
  Accepts data from Client to local registers
*/
void myClass::readFromClient() {
	ctrlValue = ModbusRTUServer.holdingRegisterRead(HRADD);
    ctrlData = ctrlValue;
}

/*
writes data to holding registers 
*/


void myClass::sendDataToClient() {
    int starting_Add = 1;
    for (int i = 0; i < 8; i++) {
        ModbusRTUServer.holdingRegisterWrite(starting_Add + i, toClientData[i]);
    }
}


/*================================================================*/
myClass myController(SLAVE_ID, BAUDRATE, HRNUM);
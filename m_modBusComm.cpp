// 
// 
// 

#include "m_modBusComm.h"

/*================================================================*/

/*
   Constructor class
   @params: slaveId, BaudRate, noOfHoldingRegsRequired
*/
myClass::myClass(int ID = 1, int baud_Rate=9600, int no_of_HR=4) {

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

int myClass::connectToClient() {
    if (!ModbusRTUServer.begin(slaveID, baudRate)) {
        return 1; //failed to connect
    }
    // configure holding register address
    ModbusRTUServer.configureHoldingRegisters(HRADD, noOfHR);

    return 0; //connected
}

/*
  Accepts data from Client to local registers
*/
void myClass::readFromClient() {
	ctrlValue = ModbusRTUServer.holdingRegisterRead(HRADD);
    if (ctrlValue > 0 && ctrlValue < 10)ctrlData = CTRL_UP;
    else if (ctrlValue >= 10 && ctrlValue < 100)ctrlData = CTRL_RIGHT;
    else if (ctrlValue >= 100 && ctrlValue < 1000)ctrlData = CTRL_DOWN;
    else if (ctrlValue >= 1000)ctrlData = CTRL_LEFT;
    else ctrlData = 0; 
}

/*
writes data to holding registers 4 - 11//steadyOFF, steadyON, TurningON, turningOFF
*/

/*
* mem: Value
* 4: INITIALISED? 
* 5: MOTORXa ON? 
* 6: MOTORXa COUNT
* 7: MOTORXb ON? 
* 8: MOTORXb COUNT
*/

void myClass::sendDataToClient() {
    int starting_Add = 1;

    for (int i = 0; i < 6; i++) {
        ModbusRTUServer.holdingRegisterWrite(starting_Add + i, toClientData[i]);
    }

}


/*================================================================*/
myClass myController(SLAVE_ID, BAUDRATE, HRNUM);
// m_modBusComm.h

#ifndef _M_MODBUSCOMM_h
#define _M_MODBUSCOMM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*
* PROGRAM CODE FOR COMMUNICATION USING MODBUS
*/

#include <ArduinoRS485.h>
#include <ArduinoModbus.h>


#define SLAVE_ID 1
#define BAUDRATE 19200

#define HRNUM 10
#define HRADD 0x00

#define NUM_CLIENT_DATA 7

#define CTRL_UP 1
#define CTRL_RIGHT 2
#define CTRL_DOWN 3
#define CTRL_LEFT 4
#define CTRL_SET_ORIGIN 10
#define CTRL_CALIBRATE_BOUNDS 11

/*================================================================*/
/* MODBUS inits-----------------------------*/
class myClass {
private:

    int ctrlValue; 
    int ctrlData; //UP, RIGHT, DOWN, LEFT Data from Control Input

    int slaveID;  //slave ID of the device
    int baudRate;
    int noOfHR; //number of holding registers required in MODBUS comm


public:
    int toClientData[10];
    int no_ClientData; 

    myClass(int, int, int);
    bool connectToClient();
    void readFromClient();

    int getCtrlData() {
        return ctrlData;
    }
    
    void poll() {
        ModbusRTUServer.poll();
    }

    void sendDataToClient(); 
};

/*----------------------------------------*/
/*================================================================*/
extern myClass myController;

#endif


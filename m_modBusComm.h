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

#define HRNUM 4
#define HRADD 0x00


/*================================================================*/
/* MODBUS inits-----------------------------*/
class myClass {
private:

    int ctrlData[4]; //UP, RIGHT, DOWN, LEFT Data from Control Input
    int posFeedback_X; //position x coordinate
    int posFeedback_Y; //position y coordinate
    int slaveID;  //slave ID of the device
    int baudRate;
    int noOfHR; //number of holding registers required in MODBUS comm
    int MotorPWM;

public:
    int toClientData[10];
    int no_ClientData; 

    myClass(int, int, int);
    int connectToClient();
    void readFromClient();

    int getCtrlData(int index) {
        return ctrlData[index];
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


/*
Sabertooth motor controller driver
Implements the packetized serial method of control
*/


#ifndef Sabertooth_h
#define Sabertooth_h
#include <Arduino.h>
typedef enum { DD_FWD = 0, DD_BACK = 1 } driveDirection_t;

#define CALC_CHECKSUM(X) X.checksum = (X.address + X.command + X.data) & 0x7F

typedef enum {
  PT_motorOneForward = 0,
  PT_motorOneBackward, //1
  PT_minVoltage, //2
  PT_maxVoltage, //3
  PT_motorTwoForward, //4
  PT_motorTwoBackward, //5
  PT_motorOneBoth, //6
  PT_motorTwoBoth, //7
  PT_mixedForward, //8
  PT_mixedBackward, //9
  PT_mixedTurnRight, //10
  PT_mixedTurnLeft, //11
  PT_mixedFwdBack, //12
  PT_mixedTurnRightLeft, //13
  PT_serialTimeout, //14
  PT_baudRate, //15
  PT_ramping, //16
  PT_deadband //17
} packetTypes_t;

typedef struct {
  unsigned char address;
  unsigned char command;
  unsigned char data;
  unsigned char checksum;
} packet_t;

typedef enum {
  BR_2400 = 1,
  BR_9600 = 2,
  BR_19200 = 3,
  BR_38400 = 4
} baudrate_t;

class Sabertooth {
	public:
    //Contructor:
    Sabertooth();
    Sabertooth(unsigned char serialPort, int baud, unsigned char address);

    //Public methods:
    void initSerialPort(void);
    void sendSetupCommands(void);
    void sendBaudCommand(baudrate_t baud); //This will sweep through the available baud rates and send the command to change it
    void updateMotor1(char speed); //-127 for max back, 0 for stop, 127 for max fwd
    void updateMotor2(char speed);
    void updateBothMotors(char speed1, char speed2);
    void sendSpeedCommands(void);
    void killMotors(void); //Kill both motors

  private:
    HardwareSerial *serialPort;
    unsigned char address;
    int baud;
    char speed1;
    char speed2;
    driveDirection_t direction1;
    driveDirection_t direction2;
};

#endif

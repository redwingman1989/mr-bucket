/*
Sabertooth motor controller driver
Implements the packetized serial method of control
*/


#ifndef SabertoothPWM_h
#define SabertoothPWM_h

#include <Servo.h>

const int PWMminCmd = 1000;
const int PWMmaxCmd = 2000;
const int PWMkillCmd = (PWMmaxCmd + PWMminCmd) / 2;

class SabertoothPWM {
	public:
    //Contructor:
    SabertoothPWM();

    //Public methods:
    void init(int motorOnePin, int motorTwoPin);
    void updateBothMotors(int speedOne, int speedTwo);
    void killMotors(void); //Kill both motors

  private:
    Servo motorOne;
    Servo motorTwo;
    char speed1;
    char speed2;
};

#endif

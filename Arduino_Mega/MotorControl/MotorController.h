/*
This module will implement the omniwheel motor command mixing
*/



#ifndef MotorControl_h
#define MotorControl_h
#include <Arduino.h>
#include <Servo.h>
#include "SabertoothPWM.h"

const float outputPowerLimit = 50.0;
const float outputCountConvert = (PWMmaxCmd - PWMminCmd) / 200.0;
const int outputOffset = (PWMmaxCmd + PWMminCmd) / 2;

//Rate limit in counts per microsecond
const float outputRateLimit = 200.0 / 1000000.0; //limit to 200 percent in 1M usec (-100 to 100 in 1 sec)

const int leftMotorPin = 5;
const int rightMotorPin = 6;
const int frontMotorPin = 3;
const int backMotorPin = 4;

typedef enum {
  M_LEFT,
  M_RIGHT,
  M_FRONT,
  M_BACK,
  M_NUM_OF_MOTORS
} motor_t;

class MotorController {
  public:
    //Constructor
    MotorController();

    //Public methods
    void init(void);
    void exec(void);
    void updateCommand(float fwdBack, float leftRight, float rotation);

    //Used for debug output
    float getCurrentMotorCommand(motor_t motor);
    int getActual(motor_t motor);

  private:

    //Assuming both are connected to serial port one
    //Assuming the left side motor is on sabertooth one, motor one
    //Assuming the right side motor is on sabertooth one, motor two
    //Assuming the front side motor is on sabertooth two, motor one
    //Assuming the back side motor is on sabertooth two, motor two
    SabertoothPWM motorLeftRight;
    SabertoothPWM motorFrontBack;

    float fwdBack;
    float leftRight;
    float rotation;

    //Array of output motor commands with range -127 (full reverse) to 127 (full forward)
    float outputCmds[M_NUM_OF_MOTORS];
    float prevOutputCmds[M_NUM_OF_MOTORS];
    int actualOutput[M_NUM_OF_MOTORS];

    unsigned long currentTime; //in microseconds
    unsigned long timeDelta; //in microseconds

};

#endif

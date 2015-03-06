/*
This module will implement the omniwheel motor command mixing
*/
#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "SabertoothPWM.h"
#include "System/RunableModule.h"
#include "Pins.h"
#include "LineSensorManager.h"

const float outputMax = 100.0;
const float outputPowerLimit = 100.0;
const float outputDeadband = 25.0;
const float zeroDeadband = 1.0;
const float deadBandScaleFactor = (outputPowerLimit - outputDeadband) / outputMax;

const float outputCountConvert = (PWMmaxCmd - PWMminCmd) / 200.0;
const int outputOffset = (PWMmaxCmd + PWMminCmd) / 2;

const float motorOffset = 4.75; //Distance from center to wheel centerline in inches

//Rate limit in percent per microsecond
//0.5 seconds to ramp 0 to 100
const float outputRateLimit = outputMax / (500000.0);

const int leftMotorPin = pinLMotorPWM;
const int rightMotorPin = pinRMotorPWM;
const int frontMotorPin = pinFMotorPWM;
const int backMotorPin = pinAMotorPWM;

typedef enum {
  M_LEFT,
  M_RIGHT,
  M_FRONT,
  M_BACK,
  M_NUM_OF_MOTORS
} motor_t;

class MotorController : public RunableModule {
  public:
    //Constructor
    MotorController();

    //Public methods
    void updateCommand(float fwdBack, float leftRight, float rotation);
    void updateCommand(float fwdBack, float leftRight, float rotation, point_t p);

    bool RunTick();
    void DebugOutput(HardwareSerial *serialPort);
    void KillMotors();

    void init(void);
  private:

    //Assuming the left side motor is on sabertooth one, motor one
    //Assuming the right side motor is on sabertooth one, motor two
    //Assuming the front side motor is on sabertooth two, motor one
    //Assuming the back side motor is on sabertooth two, motor two
    SabertoothPWM motorLeftRight;
    SabertoothPWM motorFrontBack;

    float fwdBack;
    float leftRight;
    float rotation;

    float individualRotation[M_NUM_OF_MOTORS];

    //Array of output motor commands with range -127 (full reverse) to 127 (full forward)
    float outputCmds[M_NUM_OF_MOTORS];
    float prevOutputCmds[M_NUM_OF_MOTORS];
    float outputDeadbandCmds[M_NUM_OF_MOTORS];
    int actualOutput[M_NUM_OF_MOTORS];

    void mixInputToOutput(void);
    void mixInputToOutputRotatePoint(void);
    void commandRateLimit(int8_t);
    void removeDeadband(int8_t);
    void calculatePWMTimes(int8_t);
    void updateShadows(int8_t);
    bool rotatePoint;

    unsigned long currentTime; //in microseconds
    unsigned long prevTime; //in microseconds
};

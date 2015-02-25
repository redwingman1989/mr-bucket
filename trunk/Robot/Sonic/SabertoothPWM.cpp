/*
SabertoothPWM motor controller driver
Implements the packetized PWM method of control
*/

#include <Arduino.h>
#include "SabertoothPWM.h"
#include "Servo.h"

const int startupDelay = 1000;
const int startupCycleTime = 40;

//Local function
inline int pwmLimit(int command);


//Constructor
SabertoothPWM::SabertoothPWM()
{
  this->motorOne = Servo();
  this->motorTwo = Servo();
}

void SabertoothPWM::init(int motorOnePin, int motorTwoPin)
{
  int i;
  bool flag;

  this->motorOne.attach(motorOnePin);
  this->motorTwo.attach(motorTwoPin);

  killMotors();

  //NEW INIT ROUTINE IDEA
  for (i = 0; i<=startupDelay ; i+=startupCycleTime) {
    delay(startupCycleTime);
    flag = !flag;
    if (flag) updateBothMotors(2000, 2000);
    else updateBothMotors(1000, 1000);
  }

//  delay(startupDelay);
//  updateBothMotors(2000, 2000);
//  delay(startupDelay);
//  updateBothMotors(1000, 1000);
//  delay(startupDelay);

  killMotors();
}

void SabertoothPWM::updateBothMotors(int speedOne, int speedTwo)
{
  speedOne = pwmLimit(speedOne);
  speedTwo = pwmLimit(speedTwo);

  this->motorOne.writeMicroseconds(speedOne);
  this->motorTwo.writeMicroseconds(speedTwo);
}

void SabertoothPWM::killMotors(void)
{
  this->motorOne.writeMicroseconds(PWMkillCmd);
  this->motorTwo.writeMicroseconds(PWMkillCmd);
}

inline int pwmLimit(int command)
{
  if (command < PWMminCmd) return PWMminCmd;
  else if (command > PWMmaxCmd) return PWMmaxCmd;
  else return command;
}

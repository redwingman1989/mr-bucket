/*
SabertoothPWM motor controller driver
Implements the packetized PWM method of control
*/

#include "SabertoothPWM.h"
#include "Servo.h"

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
  this->motorOne.attach(motorOnePin);
  this->motorTwo.attach(motorTwoPin);

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

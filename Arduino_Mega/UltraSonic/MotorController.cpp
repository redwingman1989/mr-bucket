/*
This module will implement the omniwheel motor command mixing
*/

#include "MotorController.h"
 HardwareSerial * temp = &Serial;
MotorController::MotorController()
{
  this->motorFrontBack = SabertoothPWM();
  this->motorLeftRight = SabertoothPWM();

  //Defualt output commands to stopped
  this->actualOutput[M_LEFT] = 0;
  this->actualOutput[M_RIGHT] = 0;
  this->actualOutput[M_FRONT] = 0;
  this->actualOutput[M_BACK] = 0;

//  this->init();
}

bool MotorController::RunTick()
{
   this->exec();
}

void MotorController::DebugOutput(HardwareSerial *serialPort)
{
  serialPort->print("DEBUG OUTPUT\n");
}

void MotorController::init(void)
{
  //Initialize the sabertooth motor controllers
  this->motorLeftRight.init(leftMotorPin, rightMotorPin);
  this->motorLeftRight.killMotors();

	this->motorFrontBack.init(frontMotorPin, backMotorPin);
	this->motorFrontBack.killMotors();

	this->timeDelta = 0;
	this->currentTime = micros();
}

void MotorController::exec(void)
{
  float temp; //Used for calculations below
  float delta;
  char i; //Iterator

  //GET TIME AND CALC EXEC RATE
  unsigned long tempTime = micros();
  this->timeDelta = tempTime - this->currentTime;
  this->currentTime = tempTime;

  //UPDATE OUTPUT COMMAND SHADOWS
  this->prevOutputCmds[M_LEFT] = this->outputCmds[M_LEFT];
  this->prevOutputCmds[M_RIGHT] = this->outputCmds[M_RIGHT];
  this->prevOutputCmds[M_FRONT] = this->outputCmds[M_FRONT];
  this->prevOutputCmds[M_BACK] = this->outputCmds[M_BACK];

  //MIXING INPUT TO OUTPUT
  temp = abs(this->rotation) + abs(this->fwdBack);

  if (temp > outputPowerLimit) {
      temp /= outputPowerLimit;
      this->outputCmds[M_LEFT] = -0.25*temp*this->rotation - 0.75*temp*this->fwdBack;
      this->outputCmds[M_RIGHT] = 0.25*temp*this->rotation - 0.75*temp*this->fwdBack;
  }
  else {
      this->outputCmds[M_LEFT] = -this->rotation - this->fwdBack;
      this->outputCmds[M_RIGHT] = this->rotation - this->fwdBack;
  }

  temp = abs(this->rotation) + abs(this->leftRight);

  if (temp > outputPowerLimit) {
      temp /= outputPowerLimit;
      this->outputCmds[M_FRONT]  = 0.25*temp*this->rotation + 0.75*temp*this->leftRight;
      this->outputCmds[M_BACK] = -0.25*temp*this->rotation + 0.75*temp*this->leftRight;
  }
  else {
      this->outputCmds[M_FRONT] = this->rotation + this->leftRight;
      this->outputCmds[M_BACK] = -this->rotation + this->leftRight;
  }

//  //OUTPUT COMMMAND RATELIMIT
//  temp = this->timeDelta * outputRateLimit;
//  for (i=M_LEFT; i<M_NUM_OF_MOTORS; i++) {
//    delta = this->outputCmds[i] - this->prevOutputCmds[i];
//    if (abs(delta) > temp)
//      if (delta > 0) this->outputCmds[i] = this->prevOutputCmds[i] + temp;
//      else this->outputCmds[i] = this->prevOutputCmds[i] - temp;
//  }

  //CONVERT TO ACTUAL OUTPUT COMMANDS
  this->actualOutput[M_LEFT] = ((int) (this->outputCmds[M_LEFT] * outputCountConvert)) + outputOffset;
  this->actualOutput[M_RIGHT] = ((int) (this->outputCmds[M_RIGHT] * outputCountConvert)) + outputOffset;
  this->actualOutput[M_FRONT] = ((int) (this->outputCmds[M_FRONT] * outputCountConvert)) + outputOffset;
  this->actualOutput[M_BACK] = ((int) (this->outputCmds[M_BACK] * outputCountConvert)) + outputOffset;

  //UPDATE AND SEND MOTOR COMMANDS
  this->motorLeftRight.updateBothMotors(this->actualOutput[M_LEFT], this->actualOutput[M_RIGHT]);
  this->motorFrontBack.updateBothMotors(this->actualOutput[M_FRONT], this->actualOutput[M_BACK]);
}

//Inputs are in percentage of max command -100% to +100%
//Forward = +
//Backward = -
//Right = +
//Left = -
//Clockwise = +
//Counter-Clockwise = -
void MotorController::updateCommand(float fwdBack, float leftRight, float rotation)
{
  //copy commands locally
  this->fwdBack = fwdBack;
  this->leftRight = leftRight;
  this->rotation = rotation;

  //apply command limits of +/- 100%
  if (this->fwdBack < -outputPowerLimit) this->fwdBack = -outputPowerLimit;
  if (this->fwdBack > outputPowerLimit) this->fwdBack = outputPowerLimit;
  if (this->leftRight < -outputPowerLimit) this->leftRight = -outputPowerLimit;
  if (this->leftRight > outputPowerLimit) this->leftRight = outputPowerLimit;
  if (this->rotation < -outputPowerLimit) this->rotation = -outputPowerLimit;
  if (this->rotation > outputPowerLimit) this->rotation = outputPowerLimit;
}

/*
This module will implement the omniwheel motor command mixing
*/
#include "MotorController.h"

MotorController::MotorController()
{
  this->motorFrontBack = SabertoothPWM();
  this->motorLeftRight = SabertoothPWM();

  //Defualt output commands to stopped
  this->actualOutput[M_LEFT] = 0;
  this->actualOutput[M_RIGHT] = 0;
  this->actualOutput[M_FRONT] = 0;
  this->actualOutput[M_BACK] = 0;

  this->prevOutputCmds[M_LEFT] = 0;
  this->prevOutputCmds[M_RIGHT] = 0;
  this->prevOutputCmds[M_FRONT] = 0;
  this->prevOutputCmds[M_BACK] = 0;
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
  //apply power limiting to inputs and save locally
  if (this->fwdBack < -outputPowerLimit) this->fwdBack = -outputPowerLimit;
  else if (this->fwdBack > outputPowerLimit) this->fwdBack = outputPowerLimit;
  else this->fwdBack = fwdBack;

  if (this->leftRight < -outputPowerLimit) this->leftRight = -outputPowerLimit;
  else if (this->leftRight > outputPowerLimit) this->leftRight = outputPowerLimit;
  else this->leftRight = leftRight;

  if (this->rotation < -outputPowerLimit) this->rotation = -outputPowerLimit;
  else if (this->rotation > outputPowerLimit) this->rotation = outputPowerLimit;
  else this->rotation = rotation;
}

void MotorController::KillMotors(void) {
  this->motorLeftRight.killMotors();
  this->motorFrontBack.killMotors();
}

void MotorController::DebugOutput(HardwareSerial *serialPort)
{
  int8_t motor;

  serialPort->print("Output: ");
  for (motor = M_LEFT ; motor < M_NUM_OF_MOTORS ; motor++) {
    serialPort->print(this->outputCmds[motor]);
    serialPort->print(" ");
  }
  serialPort->println();

}

void MotorController::init(void)
{
  //Initialize the sabertooth motor controllers
  this->motorLeftRight.init(leftMotorPin, rightMotorPin);
  this->motorLeftRight.killMotors();

	this->motorFrontBack.init(frontMotorPin, backMotorPin);
	this->motorFrontBack.killMotors();

  this->prevTime = micros();
  this->currentTime = micros();
}

bool MotorController::RunTick()
{
  int8_t motor;

  //Update time shadow and grab current time
  this->prevTime = this->currentTime;
  this->currentTime = micros();

  //MIXING INPUT TO OUTPUT
  this->mixInputToOutput();

  //LOOP THROUGH ALL THE MOTORS
  for (motor = M_LEFT; motor < M_NUM_OF_MOTORS; motor++) {
    //OUTPUT COMMMAND RATELIMIT
    this->commandRateLimit(motor);

    //REMOVE DEADBAND
    this->removeDeadband(motor);

    //CONVERT TO ACTUAL OUTPUT COMMANDS
    this->calculatePWMTimes(motor);

    //UPDATE OUTPUT COMMAND SHADOWS
    this->updateShadows(motor);
  }

  //UPDATE AND SEND MOTOR COMMANDS
  this->motorLeftRight.updateBothMotors(this->actualOutput[M_LEFT], this->actualOutput[M_RIGHT]);
  this->motorFrontBack.updateBothMotors(this->actualOutput[M_FRONT], this->actualOutput[M_BACK]);
}

void MotorController::mixInputToOutput(void) {
  float temp;
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
}

void MotorController::commandRateLimit(int8_t motor) {
  unsigned long timeDelta;
  float cmdDelta;
  float outputLimit;

  timeDelta = this->currentTime - this->prevTime;
  cmdDelta = this->outputCmds[motor] - this->prevOutputCmds[motor];
  outputLimit = timeDelta * outputRateLimit;

  if (abs(cmdDelta) > outputLimit) {
    if (cmdDelta > 0) this->outputCmds[motor] = this->prevOutputCmds[motor] + outputLimit;
    else this->outputCmds[motor] = this->prevOutputCmds[motor] - outputLimit;
  }
}

void MotorController::removeDeadband(int8_t motor) {
  this->outputDeadbandCmds[motor] = this->outputCmds[motor] * deadBandScaleFactor;
  if (this->outputCmds[motor] > 0) this->outputDeadbandCmds[motor] += outputDeadband;
  else this->outputDeadbandCmds[motor] -= outputDeadband;
}

void MotorController::calculatePWMTimes(int8_t motor) {
  this->actualOutput[motor] = ((int) (this->outputDeadbandCmds[motor] * outputCountConvert)) + outputOffset;
}

void MotorController::updateShadows(int8_t motor) {
  this->prevOutputCmds[motor] = this->outputCmds[motor];
}

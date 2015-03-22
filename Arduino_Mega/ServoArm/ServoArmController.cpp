#include "ServoArmController.h"
#include <Arduino.h>
#include <Servo.h>

const int pwmMin = 1000;
const int pwmMax = 2000;
const int pwmCenter = (pwmMax + pwmMin) / 2;

const int updateRate = 50; //50 hz exec call rate
const int swingArmLimit = (pwmMax - pwmMin) / (3 * updateRate); // full movement in five seconds
const int pickupLimit = (pwmMax - pwmMin) / (1 * updateRate); // full movement in one seconds

inline int rateLimit(int command, int current, int rate);

ServoArmController::ServoArmController()
{
  //Setup servo constants
  this->swingArmConstants[SA_DOWN] = 660;
  this->swingArmConstants[SA_UP] = 1540;

  this->pickupConstants[PU_LEFT][SA_DOWN][PS_LETGO] = 1515;
  this->pickupConstants[PU_LEFT][SA_DOWN][PS_GRAB] = 1670;
  this->pickupConstants[PU_LEFT][SA_UP][PS_LETGO] = 1585;
  this->pickupConstants[PU_LEFT][SA_UP][PS_GRAB] = 1755;

  this->pickupConstants[PU_CENTER][SA_DOWN][PS_LETGO] = 1420;
  this->pickupConstants[PU_CENTER][SA_DOWN][PS_GRAB] = 1595;
  this->pickupConstants[PU_CENTER][SA_UP][PS_LETGO] = 1507;
  this->pickupConstants[PU_CENTER][SA_UP][PS_GRAB] = 1682;

  this->pickupConstants[PU_RIGHT][SA_DOWN][PS_LETGO] = 1645;
  this->pickupConstants[PU_RIGHT][SA_DOWN][PS_GRAB] = 1840;
  this->pickupConstants[PU_RIGHT][SA_UP][PS_LETGO] = 1750;
  this->pickupConstants[PU_RIGHT][SA_UP][PS_GRAB] = 1900;

  this->swingArmCommand = this->swingArmConstants[SA_DOWN];
  this->pickupLeftCommand = this->pickupConstants[PU_LEFT][SA_DOWN][PS_LETGO];
  this->pickupCenterCommand = this->pickupConstants[PU_CENTER][SA_DOWN][PS_LETGO];
  this->pickupRightCommand = this->pickupConstants[PU_RIGHT][SA_DOWN][PS_LETGO];
}

void ServoArmController::init(void)
{
  //Setup servo pins
  this->pickupLeft.attach(38);
  this->pickupCenter.attach(39);
  this->pickupRight.attach(40);
  this->swingArm.attach(41);
  exec();
}

void ServoArmController::commandSwingArm(swingArmStates_t state)
{
  this->currentSwingArmState = state;
}

void ServoArmController::commandPickupServo(pickupServoSelection_t selection, pickupStates_t state)
{
  this->currentPickupState[selection] = state;
}

void ServoArmController::exec()
{
  //Update and rate limit servo commands
  this->swingArmCommand = rateLimit(
    this->swingArmConstants[this->currentSwingArmState],
    this->swingArmCommand,
    swingArmLimit);

  this->pickupLeftCommand = rateLimit(
    this->pickupConstants[PU_LEFT][this->currentSwingArmState][this->currentPickupState[PU_LEFT]],
    this->pickupLeftCommand,
    pickupLimit);

  this->pickupCenterCommand = rateLimit(
    this->pickupConstants[PU_CENTER][this->currentSwingArmState][this->currentPickupState[PU_CENTER]],
    this->pickupCenterCommand,
    pickupLimit);

   this->pickupRightCommand = rateLimit(
    this->pickupConstants[PU_RIGHT][this->currentSwingArmState][this->currentPickupState[PU_RIGHT]],
    this->pickupRightCommand,
    pickupLimit);

  //Write servo commands
  this->swingArm.writeMicroseconds(this->swingArmCommand);
  this->pickupLeft.writeMicroseconds(this->pickupLeftCommand);
  this->pickupCenter.writeMicroseconds(this->pickupCenterCommand);
  this->pickupRight.writeMicroseconds(this->pickupRightCommand);
  
}

inline int rateLimit(int command, int current, int rate)
{
  if ((command - current) > rate) return (current + rate);
  else if ((current - command) > rate) return (current - rate);
  else return command;
  //return command;
}

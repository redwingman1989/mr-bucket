#include "primaryStateMachine.h"
#include "MainExecMachine.h"
#include "DoublePointsStateMachine.h"
#include "Globals.h"

PrimaryStateMachine::PrimaryStateMachine() {
  currentState = (state) &PrimaryStateMachine::waitForStart;
}


void PrimaryStateMachine::waitForStart(bool firstTime) {
  static bool dpServoCmd = false;
  static bool debounce1 = false;
  static bool buttOneDetected = false;
  static bool buttTwoDetected = false;
  static bool calibrated = false;

  //NO RINGS IN THE TOP FOR QUALIFYING!!!!
  if (firstTime) arm.commandDoublePointServo(DP_DROP);

  if((buttMan.getButtons() & 0x01) && !debounce1) {
    dpServoCmd = !dpServoCmd;
    arm.commandDoublePointServo((doublePointStates_t)dpServoCmd);
    debounce1 = true;
  }
  else if (debounce1 && ((buttMan.getButtons() & 0x01) == 0x00)) {
    debounce1 = false;
  }

  /* Calibration requires orienting the robot in the score position,
   * then pressing the right front button
   * then orienting the robot in the loading heading
   * High to low trigger on the left button will commence normal operation.
   */
  if(buttMan.getButtons() & 0x02) {
    goToWork.setScoreHead(mag.getFiltHead());
    buttOneDetected = true;
  }
  if(buttOneDetected && ((buttMan.getButtons() & 0x02) == 0)) {
    calibrated = true;
  }
  if((buttMan.getButtons() & 0x04) && calibrated) {
    goToWork.setLoadHead(mag.getFiltHead());
    buttTwoDetected = true;
  }
  if (((buttMan.getButtons() & 0x04) == 0) && buttTwoDetected) {
    arm.commandDoublePointServo(DP_RETRACT);
    currentState = (state) &PrimaryStateMachine::kickSomeAss;
    runTimeStart = micros();

  }
}

void PrimaryStateMachine::kickSomeAss(bool firstTime) {

  goToWork.RunTick();

  //keep in the double down lockout in case we are really close to scoring again
  //maybe they won't notice the time....
  if (((micros() - runTimeStart) > minutes_3) && (!goToWork.lockoutDblDwn())) {
    //transition to you won instead since this is for qualifying
    currentState = (state) &PrimaryStateMachine::youWon;
  }
}

void PrimaryStateMachine::doubleDown(bool firstTime) {
  bankBreaker.loadHeading = goToWork.loadHeading;
  bankBreaker.scoreHeading = goToWork.scoreHeading;

  bankBreaker.RunTick();

  if ((micros() - runTimeStart) > (minutes_3)) {
    currentState = (state) &PrimaryStateMachine::youWon;
  }

}

void PrimaryStateMachine::youWon(bool firstTime) {
    wheels.KillMotors();
    while (1);
}

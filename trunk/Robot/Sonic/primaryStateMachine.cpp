#include "primaryStateMachine.h"
#include "MainExecMachine.h"
#include "Globals.h"

PrimaryStateMachine::PrimaryStateMachine() {
  currentState = (state) &PrimaryStateMachine::waitForStart;
}


void PrimaryStateMachine::waitForStart(bool firstTime) {
  static bool buttOneDetected = false;
  static bool buttTwoDetected = false;
  static bool calibrated = false;

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
    arm.commandDoublePointServo(DP_RETRACT);
  }
  if((buttMan.getButtons() & 0x01) && calibrated) {
    goToWork.setLoadHead(mag.getFiltHead());
    buttTwoDetected = true;
  }
  if (((buttMan.getButtons() & 0x01) == 0) && buttTwoDetected) {
    currentState = (state) &PrimaryStateMachine::kickSomeAss;
    runTimeStart = micros();

  }
}

void PrimaryStateMachine::kickSomeAss(bool firstTime) {

  goToWork.RunTick();
 // goToWork.DebugOutput(&Serial2);

  if (((micros() - runTimeStart) > 10000000) && (!goToWork.lockoutDblDwn())) {
    currentState = (state) &PrimaryStateMachine::doubleDown;
  }
}

void PrimaryStateMachine::doubleDown(bool firstTime) {
  bankBreaker.loadHeading = goToWork.loadHeading;
  bankBreaker.scoreHeading = goToWork.scoreHeading;

  bankBreaker.RunTick();

  if (micros() - runTimeStart > minutes_3) {
    currentState = (state) &PrimaryStateMachine::youWon;
  }

}

void PrimaryStateMachine::youWon(bool firstTime) {
    wheels.KillMotors();
    while (1);
}

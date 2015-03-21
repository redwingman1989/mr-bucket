#include "primaryStateMachine.h"
#include "Globals.h"

PrimaryStateMachine::PrimaryStateMachine() {
  currentState = (state) &PrimaryStateMachine::waitForStart;
}


void PrimaryStateMachine::waitForStart(bool firstTime) {
  static bool calibrated = false;

  /* Calibration requires orienting the robot in the score position,
   * then pressing the right front button
   * then orienting the robot in the loading heading
   * High to low trigger on the left button will commence normal operation.
   */
  if(buttMan.getL2HTrans() & 0x02) {
    goToWork.setScoreHead(mag.getFiltHead());
  }
  if(buttMan.getH2LTrans() & 0x02) {

    calibrated = true;
  }
  if(buttMan.getH2LTrans() & 0x01 && calibrated) {
    goToWork.setLoadHead(mag.getFiltHead());
    currentState = (state) &PrimaryStateMachine::kickSomeAss;
    runTimeStart = micros();
  }
}

void PrimaryStateMachine::kickSomeAss(bool firstTime) {

  goToWork.RunTick();
 // goToWork.DebugOutput(&Serial2);

//  if (micros() - runTimeStart > minutes_3) {
//    currentState = (state) &PrimaryStateMachine::youWon;
//  }
}

void PrimaryStateMachine::youWon(bool firstTime) {
    wheels.KillMotors();
    while (1);
}

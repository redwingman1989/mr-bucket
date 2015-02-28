#include "primaryStateMachine.h"
#include "Globals.h"

PrimaryStateMachine::PrimaryStateMachine() {
  currentState = (state) &PrimaryStateMachine::waitForStart;
}


void PrimaryStateMachine::waitForStart(bool firstTime) {

  if(buttMan.getH2LTrans() & 0x01) {
    currentState = (state) &PrimaryStateMachine::kickSomeAss;
    runTimeStart = micros();
  }
}

void PrimaryStateMachine::kickSomeAss(bool firstTime) {

  goToWork.RunTick();

  if (micros() - runTimeStart > minutes_3) {
    currentState = (state) &PrimaryStateMachine::youWon;
  }
}

void PrimaryStateMachine::youWon(bool firstTime) {
    wheels.KillMotors();
    while (1);
}

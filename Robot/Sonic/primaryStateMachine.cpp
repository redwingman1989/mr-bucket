#include "primaryStateMachine.h"
#include "Globals.h"

PrimaryStateMachine::PrimaryStateMachine() {
  currentState = (state) &PrimaryStateMachine::waitForStart;
}


void PrimaryStateMachine::waitForStart(bool firstTime) {

  if(buttMan.getH2LTrans() & 0x04) {
    currentState = (state) &PrimaryStateMachine::kickSomeAss;
  }
}

void PrimaryStateMachine::kickSomeAss(bool firstTime) {
  if (firstTime) {
    runTimeStart = micros();
  }

  goToWork.RunTick();

  if (micros() - runTimeStart > minutes_3) {
    currentState = (state) &PrimaryStateMachine::youWon;
  }
}

void PrimaryStateMachine::youWon(bool firstTime) {
  if (firstTime) {
    while (1);
  }
}

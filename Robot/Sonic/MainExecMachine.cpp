#include "MainExecMachine.h"
#include "StateMachine.h"
#include "Globals.h"
#include "driveAlgorithms.h"
#include "stateMachineCommon.h"

MainExecMachine::MainExecMachine() {
  currentState = (state) &MainExecMachine::loadLeftRightRings;
  stateNum = MEST_LOAD_LR_RINGS;
}

void MainExecMachine::DebugOutput(HardwareSerial * serialPort){
  serialPort->print("st#: ");
  serialPort->println(stateNum);
}

void MainExecMachine::loadLeftRightRings(bool first) {

}

void MainExecMachine::backupFromLeftRightRings(bool first) {

}

void MainExecMachine::shiftForCenterRings(bool first) {

}

void MainExecMachine::loadCenterRings(bool first) {

}

void MainExecMachine::backupFromCenterRings(bool first) {

}

void MainExecMachine::raiseArm(bool first) {

}

void MainExecMachine::flipToScore(bool first) {

}

void MainExecMachine::findCenterLineToScore(bool first) {
  static bool firstTime = true;

  if (findCenterLine(firstTime)) {
    firstTime = true;

    stateNum = MEST_HAUL_TOSCORE;
    currentState = (state) &MainExecMachine::haulToScore;
  }

  else firstTime = false;
}

void MainExecMachine::haulToScore(bool first) {
  if (haulAss(first)) {
    stateNum = MEST_SCORE;
    currentState = (state) &MainExecMachine::scoreRings;
  }
}

void MainExecMachine::scoreRings(bool first) {

}

void MainExecMachine::backupFromScoring(bool first) {

}

void MainExecMachine::lowerArm(bool first) {

}

void MainExecMachine::flipToLoad(bool first) {

}

void MainExecMachine::findCenterLineToLoad(bool first) {
  static bool firstTime = true;

  if (findCenterLine(firstTime)) {
    firstTime = true;

    stateNum = MEST_HAUL_TOLOAD;
    currentState = (state) &MainExecMachine::haulToLoad;
  }

  else firstTime = false;
}

void MainExecMachine::haulToLoad(bool first) {
  if (haulAss(first)) {
    stateNum = MEST_LOAD_LR_RINGS;
    currentState = (state) &MainExecMachine::loadLeftRightRings;
  }
}

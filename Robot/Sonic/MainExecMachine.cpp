#include "MainExecMachine.h"
#include "StateMachine.h"
#include "Globals.h"
#include "driveAlgorithms.h"

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

}

void MainExecMachine::haulToScore(bool first) {

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

}

void MainExecMachine::haulToLoad(bool first) {

}

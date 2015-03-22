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
  wheels.updateCommand(-2, 0, 0);
  FollowLine(0, -2, LSP_RIGHT);
  if (lineManager.getLineDriveCommand(LSP_BACK).valid ||
      ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 5)
  {
    wheels.updateCommand(0,0,0);
    stateNum = MEST_SHIFT_FOR_CENTER;
    currentState = (state) &MainExecMachine::shiftForCenterRings;
  }
}

void MainExecMachine::shiftForCenterRings(bool first) {
  static bool centerSensorCount = false;
  static bool rightSensorCount = false;
  static bool needToCenter = false;
  static int lineUpCount = 0;

  wheels.updateCommand(0, 3, 0);

  if(lineManager.getLineDriveCommand(LSP_CENTER).valid) {
    centerSensorCount = true;
  }

  if(centerSensorCount && lineManager.getLineDriveCommand(LSP_RIGHT).valid) {
    rightSensorCount = true;
  }

  if (centerSensorCount && rightSensorCount &&
      lineManager.getLineDriveCommand(LSP_CENTER).valid) {
    needToCenter = true;
  }

  if (needToCenter) {
    if(FollowLineSingle(0,true,LSL_CENTER_FRONT)) {
            if(lineUpCount++ > 10){
                wheels.updateCommand(0,0,0);
                stateNum = MEST_LOAD_CENTER_RINGS;
                currentState = (state) &MainExecMachine::loadCenterRings;
                centerSensorCount = false;
                rightSensorCount = false;
                needToCenter = false;
                lineUpCount=0;
            }
    } else{
        lineUpCount = 0;
    }
  }
}

void MainExecMachine::loadCenterRings(bool first) {

}

void MainExecMachine::backupFromCenterRings(bool first) {
  wheels.updateCommand(-20, 0, 0);
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
  {
    wheels.updateCommand(0,0,0);
    stateNum = MEST_RAISE_ARM;
    currentState = (state) &MainExecMachine::raiseArm;
  }
}

void MainExecMachine::raiseArm(bool first) {
  arm.commandSwingArm(SA_UP);
  stateNum = MEST_FLIP_ONE;
  currentState = (state) &MainExecMachine::flipToScore;
}

void MainExecMachine::flipToScore(bool first) {
  if (flipABitch(scoreHeading)) {
    stateNum = MEST_FIND_CENTER_LINE_ONE;
    currentState = (state) &MainExecMachine::findCenterLineToScore;
  }
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
  wheels.updateCommand(-20, 0, 0);
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
  {
    wheels.updateCommand(0,0,0);
    stateNum = MEST_LOWER_ARM;
    currentState = (state) &MainExecMachine::lowerArm;
  }
}

void MainExecMachine::lowerArm(bool first) {
  arm.commandSwingArm(SA_DOWN);
  stateNum = MEST_FLIP_TWO;
  currentState = (state) &MainExecMachine::flipToLoad;
}

void MainExecMachine::flipToLoad(bool first) {
  if (flipABitch(loadHeading)) {
    stateNum = MEST_FIND_CENTER_LINE_TWO;
    currentState = (state) &MainExecMachine::findCenterLineToLoad;
  }
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

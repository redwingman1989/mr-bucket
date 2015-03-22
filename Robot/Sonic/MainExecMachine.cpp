#include "MainExecMachine.h"
#include "StateMachine.h"
#include "stateMachineCommon.h"
#include "Globals.h"
#include "driveAlgorithms.h"
#include "stateMachineCommon.h"
#include "SweepExecMachine.h"

MainExecMachine::MainExecMachine() {
  zamboniLoadZone = SweepExecMachine(Z_LOAD);
  zamboniScoreZone = SweepExecMachine(Z_SCORE);
  currentState = (state) &MainExecMachine::loadLeftRightRings;
  stateNum = MEST_LOAD_LR_RINGS;
  sharedData.staticButtonsDetected = false;
  sharedData.staticButtShadow = 0;
  sharedData.staticButtTimeout = 0;
  sharedData.staticStateTimeout = micros();
  sharedData.staticButtonTimeoutFlag = false;
}

void MainExecMachine::DebugOutput(HardwareSerial * serialPort){
  serialPort->print("st#: ");
  serialPort->println(stateNum);
}

bool MainExecMachine::RunTick() {
  if (loadZoneDirty && (currentState == &MainExecMachine::loadLeftRightRings)) {
    loadZoneDirty = !zamboniLoadZone.RunTick();
  }
  else if (scoreZoneDirty && (currentState == &MainExecMachine::scoreRings)){
    scoreZoneDirty = !zamboniScoreZone.RunTick();
  }
  else {
    runCurrentState();
  }
}

void MainExecMachine::loadLeftRightRings(bool first) {
  static bool firstTime = true;

  if (firstTime) {
    sharedData.staticStateTimeout = micros();
    firstTime = false;
  }

  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_RIGHT_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Reset the first time flag  */
    firstTime = true;
    /* Move on to the next state */
    clearStaticData(&sharedData);
    currentState = (state) &MainExecMachine::pickupLeftRightRings;
  }

  /* A button has not been pressed for 5 seconds and the robot is more
   *   than an inch from the wall */
  if (sharedData.staticButtonTimeoutFlag) {
    /* Mark Loading Zone as dirty */
    loadZoneDirty = true;
    firstTime = true;
    /* Clear Static Data */
    clearStaticData(&sharedData);
    /* DO NOT CHANGE STATES */
  }
}

void MainExecMachine::pickupLeftRightRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }

  wheels.updateCommand(0, 0, 0);
  loadHeading = mag.getFiltHead();
  arm.commandPickupServo(PU_LEFT, PS_GRAB);
  arm.commandPickupServo(PU_RIGHT, PS_GRAB);

  if (micros() - pickupStartTime > ringLoadTime) {
    firstTime = true;
    stateNum = MEST_BACKUP_ONE;
    currentState = (state) &MainExecMachine::backupFromLeftRightRings;
  }

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

  static bool firstTime = true;

  if (firstTime) {
    sharedData.staticStateTimeout = micros();
    firstTime = false;
  }

  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_CENTER_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Reset the first time flag  */
    firstTime = true;
    clearStaticData(&sharedData);
    /* Move on to the next state */
    currentState = (state) &MainExecMachine::pickupCenterRings;
  }

  /* A button has not been pressed for 5 seconds and the robot is more
   *   than an inch from the wall */
  if (sharedData.staticButtonTimeoutFlag) {
    /* Mark Loading Zone as dirty */
    loadZoneDirty = true;
    /* Clear Static Data */
    clearStaticData(&sharedData);
    /* THIS ONE NEEDS TO CHANGE STATES */
    currentState = (state) &MainExecMachine::pickupCenterRings;
  }

}

void MainExecMachine::pickupCenterRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }

  wheels.updateCommand(0,0,0);
  loadHeading = mag.getFiltHead();
  arm.commandPickupServo(PU_CENTER, PS_GRAB);

  if (micros() - pickupStartTime > ringLoadTime) {
    firstTime = true;
    stateNum = MEST_BACKUP_TWO;
    currentState = (state) &MainExecMachine::backupFromCenterRings;
  }
}

void MainExecMachine::backupFromCenterRings(bool first) {
  wheels.updateCommand(-20, 0, 0);
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 12)
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
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(scoreHeading);
  // once the desired heading is reached
  //   transition to findCenterLin
  rotationSpeed =  getToHeadingDirection(scoreHeading,true);

  if(abs(rotationSpeed) < 1 || abs(delta) < 3){
    if(lineUpCount++ > 10){
        lineUpCount = 0;
        stateNum = MEST_FIND_CENTER_LINE_ONE;
        currentState = (state) &MainExecMachine::findCenterLineToScore;
    }
  }
  else {
        point_t point= {2,0};
        wheels.updateCommand(0,0,rotationSpeed,point);
        lineUpCount = 0;
  }
}

void MainExecMachine::findCenterLineToScore(bool first) {
  static bool firstTime = true;

  if (findCenterLine(firstTime, 15, 5, 4)) {
    firstTime = true;

    stateNum = MEST_HAUL_TOSCORE;
    currentState = (state) &MainExecMachine::haulToScore;
  }

  else firstTime = false;
}

void MainExecMachine::haulToScore(bool first) {
  static int8_t sideSpeed;
  static bool lostLine = false;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  sideSpeed = 2*(rightError - leftError);

  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    findCenterLine(!lostLine, 0, 5, 4);
    lostLine = true;
  }

  else if (frontDist > 15) {
    forwardSpeed = (frontDist-15) + 2;
    forwardSpeed = forwardSpeed > 50 ? 50 : forwardSpeed;
    forwardSpeed = forwardSpeed < 2 ? 2 : forwardSpeed;
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
  }

  else {
    wheels.updateCommand(0, 0, 0);
    lostLine = false;
    stateNum = MEST_SCORE;
    currentState = (state) &MainExecMachine::scoreRings;
  }
}

void MainExecMachine::scoreRings(bool first) {
  static bool firstTime = true;

  if (firstTime) {
    sharedData.staticStateTimeout = micros();
    firstTime = false;
  }

  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_RIGHT_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Reset the first time flag  */
    firstTime = true;
    /* Clear Static Data */
    clearStaticData(&sharedData);
    /* Move on to the next state */
    currentState = (state) &MainExecMachine::unloadAllRings;
  }

  /* A button has not been pressed for 5 seconds and the robot is more
   *   than an inch from the wall */
  if (sharedData.staticButtonTimeoutFlag) {
    /* Mark Loading Zone as dirty */
    scoreZoneDirty = true;
    firstTime = true;
    /* Clear Static Data */
    clearStaticData(&sharedData);
    /* DO NOT CHANGE STATES */
  }
}

void MainExecMachine::unloadAllRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }
  wheels.updateCommand(0,0,0);
  scoreHeading = mag.getFiltHead();
  arm.commandPickupServo(PU_LEFT, PS_LETGO);
  arm.commandPickupServo(PU_CENTER, PS_LETGO);
  arm.commandPickupServo(PU_RIGHT, PS_LETGO);

  if (micros() - pickupStartTime > ringLoadTime) {
    firstTime = true;
    stateNum = MEST_BACKUP_THREE;
    currentState = (state) &MainExecMachine::backupFromScoring;
  }
}

void MainExecMachine::backupFromScoring(bool first) {
  wheels.updateCommand(-20, 0, 0);
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 12)
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
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(loadHeading);
  // once the desired heading is reached
  //   transition to findCenterLin
  rotationSpeed =  getToHeadingDirection(loadHeading,true);

  if(abs(rotationSpeed) < 1 || abs(delta) < 3){
    if(lineUpCount++ > 10){
        lineUpCount = 0;
        stateNum = MEST_FIND_CENTER_LINE_TWO;
        currentState = (state) &MainExecMachine::findCenterLineToLoad;
    }
  }
  else {
        point_t point= {3.5,0};
        wheels.updateCommand(0,0,rotationSpeed,point);
        lineUpCount = 0;
  }
}

void MainExecMachine::findCenterLineToLoad(bool first) {
  static bool firstTime = true;

  if (findCenterLine(firstTime, 15, 5, 4)) {
    firstTime = true;

    stateNum = MEST_HAUL_TOLOAD;
    currentState = (state) &MainExecMachine::haulToLoad;
  }

  else firstTime = false;
}

void MainExecMachine::haulToLoad(bool first) {
  static int8_t sideSpeed;
  static bool lostLine = false;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  sideSpeed = 2*(rightError - leftError);

  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    findCenterLine(!lostLine, 0, 5, 4);
    lostLine = true;
  }

  else if (frontDist > 15) {
    forwardSpeed = (frontDist-15) + 2;
    forwardSpeed = forwardSpeed > 50 ? 50 : forwardSpeed;
    forwardSpeed = forwardSpeed < 2 ? 2 : forwardSpeed;
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
  }

  else {
    wheels.updateCommand(0, 0, 0);
    lostLine = false;
    stateNum = MEST_LOAD_LR_RINGS;
    currentState = (state) &MainExecMachine::loadLeftRightRings;
  }
}

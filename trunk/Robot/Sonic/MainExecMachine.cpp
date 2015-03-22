#include "MainExecMachine.h"
#include "StateMachine.h"
#include "stateMachineCommon.h"
#include "Globals.h"
#include "driveAlgorithms.h"
#include "stateMachineCommon.h"

static loadRingsSharedStaticData_t sharedData = {false, 0, 0, 0};

MainExecMachine::MainExecMachine() {
  currentState = (state) &MainExecMachine::loadLeftRightRings;
  stateNum = MEST_LOAD_LR_RINGS;
}

void MainExecMachine::DebugOutput(HardwareSerial * serialPort){
  serialPort->print("st#: ");
  serialPort->println(stateNum);
}

void MainExecMachine::loadLeftRightRings(bool first) {
  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_RIGHT_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Move Servos */
    arm.commandPickupServo(PU_LEFT, PS_GRAB);
    arm.commandPickupServo(PU_RIGHT, PS_GRAB);
    /* Move on to the next state */
    currentState = (state) &MainExecMachine::backupFromLeftRightRings;
  }

  /* Call the common Load Rings timeout check function */
  if (loadRingsTimeOutCheck(&sharedData)) {
    /* Set the new desired heading */
    desiredHeading = scoreHeading;
    /* If we timed out, move on */
    currentState = (state) &MainExecMachine::backupFromLeftRightRings;
  }

  /* We may need to update the heading */
  if (sharedData.staticButtShadow & 0x3 == 0x3) {
    loadHeading = mag.getFiltHead();
  }
}

void MainExecMachine::pickupLeftRightRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }

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
  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_CENTER_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Move Servos */
    arm.commandPickupServo(PU_CENTER, PS_GRAB);
    /* Move to the next state */
    currentState = (state) &MainExecMachine::backupFromCenterRings;
  }

  /* Call the common Load Rings timeout check function */
  if (loadRingsTimeOutCheck(&sharedData)) {
    /* Set the new desired heading */
    desiredHeading = scoreHeading;
    /* If we timed out, move on */
    currentState = (state) &MainExecMachine::backupFromCenterRings;
  }

  /* We may need to update the heading */
  if (sharedData.staticButtShadow & 0x3 == 0x3) {
    loadHeading = mag.getFiltHead();
  }
}

void MainExecMachine::pickupCenterRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }

  arm.commandPickupServo(PU_CENTER, PS_GRAB);

  if (micros() - pickupStartTime > ringLoadTime) {
    firstTime = true;
    stateNum = MEST_BACKUP_TWO;
    currentState = (state) &MainExecMachine::backupFromCenterRings;
  }
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
        point_t point= {3.5,0};
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
  /* Call the common Load Rings functionality */
  loadRingsButtonDetection(LSL_RIGHT_FRONT, &sharedData);

  /* Perform Arm Movements based on if buttons were detected */
  if (sharedData.staticButtonsDetected) {
    /* Move Servos */
    arm.commandPickupServo(PU_LEFT, PS_LETGO);
    arm.commandPickupServo(PU_RIGHT, PS_LETGO);
    arm.commandPickupServo(PU_CENTER, PS_LETGO);
    /* Move on to the next state */
    currentState = (state) &MainExecMachine::backupFromScoring;
  }

  /* Call the common Load Rings timeout check function */
  if (loadRingsTimeOutCheck(&sharedData)) {
    /* Set the new desired heading */
    desiredHeading = loadHeading;
    /* If we timed out, move on */
    currentState = (state) &MainExecMachine::backupFromScoring;
  }

  /* We may need to update the heading */
  if (sharedData.staticButtShadow & 0x3 == 0x3) {
    scoreHeading = mag.getFiltHead();
  }
}

void MainExecMachine::unloadAllRings(bool first) {
  static bool firstTime = true;
  static unsigned long pickupStartTime = 0;

  if (firstTime) {
    pickupStartTime = micros();
    firstTime = false;
  }

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

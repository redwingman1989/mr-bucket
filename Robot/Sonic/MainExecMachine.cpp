#include "MainExecMachine.h"
#include "stateMachineCommon.h"
#include "Globals.h"
#include "driveAlgorithms.h"
#include "SweepExecMachine.h"

MainExecMachine::MainExecMachine() {
  zamboniLoadZone = SweepExecMachine(Z_LOAD);
  zamboniScoreZone = SweepExecMachine(Z_SCORE);
  currentState = (state) &MainExecMachine::loadLeftRightRings;
  stateNum = MEST_LOAD_LR_RINGS;
  loadZoneDirty = false;
  scoreZoneDirty = false;
  lockoutDoubleDown = false;
}

void MainExecMachine::DebugOutput(HardwareSerial * serialPort){
  serialPort->print("st#: ");
  serialPort->println(stateNum);
}

bool MainExecMachine::RunTick() {
  /* If we changed states, allow for higher rate limiting in the motor controller */
  if(prevState != currentState) {
    wheels.setHigherRateLimiting();
  }

  if (loadZoneDirty && (currentState == &MainExecMachine::loadLeftRightRings)) {
    loadZoneDirty = !zamboniLoadZone.RunTick();
  }
  else if (scoreZoneDirty && (currentState == &MainExecMachine::scoreRings)){
    scoreZoneDirty = !zamboniScoreZone.RunTick();
  }
  else {
    runCurrentState();
  }
  return false;
}

void MainExecMachine::loadLeftRightRings(bool first) {
  static bool firstTime = false;
  static bool buttItTOStartFlag = false;
  static uint8_t buttShadow = 0;
  static uint32_t stateStartTime = micros();
  static uint8_t buttItTOCntr = 0;

  float frontWallDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  int rotation = 0;

  const uint8_t buttItTimeout = 50; //50 iterations @ 100Hz = 0.5 seconds

  /* If the first time, set the state start time */
  if (firstTime) {
    stateStartTime = micros();
    firstTime = false;
  }

  buttShadow |= buttMan.getButtons();

  /* Check to see if the button iteration timeout has been started */
    /* If timeout has not been started, look at buttons to see if we should start it */
    if ((buttShadow & 0x03) > 0){
      buttItTOStartFlag = true;
    }

  /* Check to see if all buttons have been pressed */
  if (buttShadow & 0x03 == 0x03) {
    /* Clean up static variable(s) and move state machine on */
    firstTime = true;
    buttItTOStartFlag = false;
    buttShadow = 0;
    stateStartTime = micros();
    buttItTOCntr = 0;

    /* Both buttons hit so mark the load zone as clean */
    loadZoneDirty = false;

    /* Move on */
    currentState = (state) &MainExecMachine::pickupLeftRightRings;
  }
  /* Else, one or none of the buttons have been pressed */
  else {
    /* If at least one button has been pressed (buttItToStartFlag will be set) */
    if (buttItTOStartFlag) {
      /* Check to see if we have timed out */
      if (buttItTOCntr++ >= buttItTimeout) {
        /* Timeout expired: we hit one button and did not hit the other one */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        /* DO NOT MARK DIRTY, even though we timed out */
        loadZoneDirty = false;

        /* Move on */
        currentState = (state) &MainExecMachine::pickupLeftRightRings;
        return;
      }
    }
    /* Else, no buttons are hit. Attempt to drive to the wall */
    else {
      /* Check for timeout */
      if ((frontWallDist <= 4) && ((micros() - stateStartTime) >= loadRingsButtonTimeout)) {
        /* A button was never pressed and we timed out being in this state */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        /* MARK DIRTY and do not move the state machine forward */
        loadZoneDirty = true;

        return;
      }

      /* We aren't less than 4 inches from the wall OR
       *   we are less than 4 inches but our state timeout hasn't expired
       *   so continue driving to the wall */
      FollowLineSingle(4,true, LSL_RIGHT_FRONT);
    }
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
  arm.setPickupArmLimit(1); //Set Rate to 1 second to swing
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
    wheels.updateCommand(0,2,0);
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
  static bool firstTime = false;
  static bool buttItTOStartFlag = false;
  static uint8_t buttShadow = 0;
  static uint32_t stateStartTime = micros();
  static uint8_t buttItTOCntr = 0;

  float frontWallDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  int rotation = 0;

  const uint8_t buttItTimeout = 50; //50 iterations @ 100Hz = 0.5 seconds

  /* If the first time, set the state start time */
  if (firstTime) {
    stateStartTime = micros();
    firstTime = false;
  }

  buttShadow |= buttMan.getButtons();

  /* Check to see if the button iteration timeout has been started */
    /* If timeout has not been started, look at buttons to see if we should start it */
    if ((buttShadow & 0x03) > 0){
      buttItTOStartFlag = true;
    }

  /* Check to see if all buttons have been pressed */
  if (buttShadow & 0x03 == 0x03) {
    /* Clean up static variable(s) and move state machine on */
    firstTime = true;
    buttItTOStartFlag = false;
    buttShadow = 0;
    stateStartTime = micros();
    buttItTOCntr = 0;

    /* Both buttons hit so mark the load zone as clean */
    loadZoneDirty = false;

    /* Move on */
    currentState = (state) &MainExecMachine::pickupCenterRings;
  }
  /* Else, one or none of the buttons have been pressed */
  else {
    /* If at least one button has been pressed (buttItToStartFlag will be set) */
    if (buttItTOStartFlag) {
      /* Check to see if we have timed out */
      if (buttItTOCntr++ >= buttItTimeout) {
        /* Timeout expired: we hit one button and did not hit the other one */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        /* DO NOT MARK DIRTY, even though we timed out */
        loadZoneDirty = false;

        /* Move on */
        currentState = (state) &MainExecMachine::pickupCenterRings;
        return;
      }

    }
    /* Else, no buttons are hit. Attempt to drive to the wall */
    else {
      /* Check for timeout */
      if ((frontWallDist <= 4) && ((micros() - stateStartTime) >= loadRingsButtonTimeout)) {
        /* A button was never pressed and we timed out being in this state */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        /* MARK DIRTY and move the state machine forward */
        loadZoneDirty = true;

        currentState = (state) &MainExecMachine::pickupCenterRings;

        return;
      }

      /* We aren't less than 4 inches from the wall OR
       *   we are less than 4 inches but our state timeout hasn't expired
       *   so continue driving to the wall */
      FollowLineSingle(4,true, LSL_CENTER_FRONT);
    }
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
  arm.setPickupArmLimit(1); //Set Rate to 1 second to swing
  arm.commandPickupServo(PU_CENTER, PS_GRAB);

  if (micros() - pickupStartTime > ringLoadTime) {
    firstTime = true;
    stateNum = MEST_BACKUP_TWO;
    currentState = (state) &MainExecMachine::backupFromCenterRings;
  }
}

void MainExecMachine::backupFromCenterRings(bool first) {
  wheels.updateCommand(-10, 0, 0);
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 12)
  {
    wheels.updateCommand(0,0,0);
    stateNum = MEST_RAISE_ARM;
    currentState = (state) &MainExecMachine::raiseArm;
  }
}

void MainExecMachine::raiseArm(bool first) {
  arm.setPickupArmLimit(3); //Set Rate to 3 seconds to swing
  arm.setSwingArmLimit(3); //Set Rate to 3 seconds to swing
  arm.commandSwingArm(SA_UP);
  stateNum = MEST_FLIP_ONE;
  currentState = (state) &MainExecMachine::flipToScore;
}

void MainExecMachine::flipToScore(bool first) {
  static bool firstTime = true;
  static uint32_t startFlipTime = micros();
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(scoreHeading);
  // once the desired heading is reached
  //   transition to findCenterLin

  if (firstTime) {
    firstTime = false;
    startFlipTime = micros();
  }

  /* Check for a state timeout */
  if (micros() - startFlipTime > flipTimeout) {
    wheels.updateCommand(0,0,0);
    firstTime = true;
    stateNum = MEST_FIND_CENTER_LINE_ONE;
    currentState = (state) &MainExecMachine::findCenterLineToScore;
  }

  rotationSpeed =  getToHeadingDirection(scoreHeading,true);

  if(abs(rotationSpeed) < 1 || abs(delta) < 3){
    if(lineUpCount++ > 10){
        lineUpCount = 0;
        stateNum = MEST_FIND_CENTER_LINE_ONE;
        currentState = (state) &MainExecMachine::findCenterLineToScore;
    }
    wheels.updateCommand(0,0,0);
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

  else  firstTime = false;

  if (abs(getDeltaHeading(scoreHeading)) > 25) {
    stateNum = MEST_FLIP_ONE;
    currentState = (state) &MainExecMachine::flipToScore;
    firstTime = true;
  }

}

void MainExecMachine::haulToScore(bool first) {
  static int8_t sideSpeed;
  static bool lostLine = false;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  const float minSpeed = 4;   // in motor command units
  const float maxSpeed = 50;  // in motor command units
  const float maxDist = 48.0; // in inches
  const float minDist = 20.0; // in inches
  const float stateTransDist = 12.0; // in inches
  const uint8_t wallThresh = 20; // iterations
  static int distanceCount = 0;
  sideSpeed = 2*(rightError - leftError);

  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    stateNum = MEST_FIND_CENTER_LINE_ONE;
    currentState = (state) &MainExecMachine::findCenterLineToScore;
  }

  else {
    forwardSpeed = scaleDistanceToSpeedCmd(frontDist,
                                           maxDist,
                                           minDist,
                                           maxSpeed,
                                           minSpeed);
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
    if(frontDist > stateTransDist)
      distanceCount = 0;
    else if(distanceCount++ > wallThresh){
      wheels.updateCommand(0, 0, 0);
      lostLine = false;
      stateNum = MEST_SCORE;
      currentState = (state) &MainExecMachine::scoreRings;
      distanceCount = 0;
    }
  }
}

void MainExecMachine::scoreRings(bool first) {
  static bool firstTime = false;
  static bool buttItTOStartFlag = false;
  static uint8_t buttShadow = 0;
  static uint32_t stateStartTime = micros();
  static uint8_t buttItTOCntr = 0;

  float frontWallDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  int rotation = 0;

  const uint8_t buttItTimeout = 50; //50 iterations @ 100Hz = 0.5 seconds

  lockoutDoubleDown = true;

  /* If the first time, set the state start time */
  if (firstTime) {
    stateStartTime = micros();
    firstTime = false;
  }

  buttShadow |= buttMan.getButtons();

  /* Check to see if the button iteration timeout has been started */
    /* If timeout has not been started, look at buttons to see if we should start it */
    if ((buttShadow & 0x03) > 0){
      buttItTOStartFlag = true;
    }

  /* Check to see if all buttons have been pressed */
  if (buttShadow & 0x03 == 0x03) {
    /* Clean up static variable(s) and move state machine on */
    firstTime = true;
    buttItTOStartFlag = false;
    buttShadow = 0;
    stateStartTime = micros();
    buttItTOCntr = 0;

    lockoutDoubleDown = true;

    /* Both buttons hit so mark the load zone as clean */
    scoreZoneDirty = false;

    /* Move on */
    currentState = (state) &MainExecMachine::unloadAllRings;
  }
  /* Else, one or none of the buttons have been pressed */
  else {
    /* If at least one button has been pressed (buttItToStartFlag will be set) */
    if (buttItTOStartFlag) {
      /* Check to see if we have timed out */
      if (buttItTOCntr++ >= buttItTimeout) {
        /* Timeout expired: we hit one button and did not hit the other one */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        lockoutDoubleDown = true;

        /* DO NOT MARK DIRTY, even though we timed out */
        scoreZoneDirty = false;

        /* Move on */
        currentState = (state) &MainExecMachine::unloadAllRings;
        return;
      }

      wheels.updateCommand(0,0,rotation);
    }
    /* Else, no buttons are hit. Attempt to drive to the wall */
    else {
      /* Check for timeout */
      if ((frontWallDist <= 4) && ((micros() - stateStartTime) >= loadRingsButtonTimeout)) {
        /* A button was never pressed and we timed out being in this state */

        /* Clean up static variables */
        firstTime = true;
        buttItTOStartFlag = false;
        buttShadow = 0;
        stateStartTime = micros();
        buttItTOCntr = 0;

        /* We are marking the scoring zone as dirty, so remove the lockout on the double down code */
        lockoutDoubleDown = false;

        /* MARK DIRTY and do not move the state machine forward */
        scoreZoneDirty = true;

        return;
      }

      /* We aren't less than 4 inches from the wall OR
       *   we are less than 4 inches but our state timeout hasn't expired
       *   so continue driving to the wall */
      FollowLineSingle(4,true, LSL_RIGHT_FRONT);
    }
  }
}


void MainExecMachine::unloadAllRings(bool first) {
  static bool firstTime = true;
  static unsigned long unloadStartTime = 0;

  lockoutDoubleDown = true;

  if (firstTime) {
    unloadStartTime = micros();
    firstTime = false;
  }

  wheels.updateCommand(0,0,0);
  scoreHeading = mag.getFiltHead();
  arm.setPickupArmLimit(1); //Set Rate to 1 seconds to swing
  arm.commandPickupServo(PU_LEFT, PS_LETGO);
  arm.commandPickupServo(PU_CENTER, PS_LETGO);
  arm.commandPickupServo(PU_RIGHT, PS_LETGO);

  if (micros() - unloadStartTime > 2000000) {
    firstTime = true;
    lockoutDoubleDown = false;
    stateNum = MEST_BACKUP_THREE;
    currentState = (state) &MainExecMachine::backupFromScoring;
  }
}

void MainExecMachine::backupFromScoring(bool first) {
  const float minSpeed = 2;   // in motor command units
  const float maxSpeed = 30;  // in motor command units
  const float maxDist = 10.0; // in inches
  const float minDist = 4.0; // in inches
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  float backwardSpeed = -scaleDistanceToSpeedCmd(frontDist,
                                           maxDist,
                                           minDist,
                                           maxSpeed,
                                           minSpeed);

  FollowLine(0, backwardSpeed ,  LSP_RIGHT);

  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > maxDist)
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
  static bool firstTime = true;
  static uint32_t flipTimeStart = micros();
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(loadHeading);
  // once the desired heading is reached
  //   transition to findCenterLin

  if (firstTime) {
    firstTime = false;
    flipTimeStart = micros();
  }

  if (micros() - flipTimeStart > flipTimeout) {
    wheels.updateCommand(0,0,0);
    firstTime = true;
    stateNum = MEST_FIND_CENTER_LINE_TWO;
    currentState = (state) &MainExecMachine::findCenterLineToLoad;
  }

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

  if (findCenterLine(firstTime, 25, 5, 4)) {
    firstTime = true;

    stateNum = MEST_HAUL_TOLOAD;
    currentState = (state) &MainExecMachine::haulToLoad;
  }

  else firstTime = false;

  if (abs(getDeltaHeading(loadHeading)) > 25) {
    stateNum = MEST_FLIP_TWO;
    currentState = (state) &MainExecMachine::flipToLoad;
    firstTime = true;
  }
}

void MainExecMachine::haulToLoad(bool first) {
  static int8_t sideSpeed;
  static bool lostLine = false;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static int distanceCount = 0;
  const float minSpeed = 4;   // in motor command units
  const float maxSpeed = 80;  // in motor command units
  const float maxDist = 48.0; // in inches
  const float minDist = 20.0; // in inches
  const float stateTransDist = 12.0; // in inches
  const uint8_t wallThresh = 20; // iterations
  sideSpeed = 2*(rightError - leftError);

  if(!lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    stateNum = MEST_FIND_CENTER_LINE_TWO;
    currentState = (state) &MainExecMachine::findCenterLineToLoad;
  }
  else {
    forwardSpeed = scaleDistanceToSpeedCmd(frontDist,
                                           maxDist,
                                           minDist,
                                           maxSpeed,
                                           minSpeed);
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
    if (frontDist > stateTransDist)
      distanceCount = 0;
    else if(distanceCount++ > wallThresh){
      wheels.updateCommand(0, 0, 0);
      lostLine = false;
      stateNum = MEST_LOAD_LR_RINGS;
      currentState = (state) &MainExecMachine::loadLeftRightRings;
      distanceCount = 0;
    }
  }
}


bool MainExecMachine::lockoutDblDwn() {
  return lockoutDoubleDown;
}

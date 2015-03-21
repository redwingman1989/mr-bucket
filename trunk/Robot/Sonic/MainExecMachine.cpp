#include "MainExecMachine.h"
#include "StateMachine.h"
#include "Globals.h"
#include "driveAlgorithms.h"

MainExecMachine::MainExecMachine() {
  currentState = (state) &MainExecMachine::loadRings;
  stateNum = MEST_LOAD_LR_RINGS;
}

void MainExecMachine::loadRings(bool firstTime) {
  static bool buttonsDetected = 0;
  const int timeBeforeButtonIgnore = 50;
  static int8_t rotation = 0;
  float distanceToFront = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  /* Move Forward Till we hit Buttons */
  static uint8_t buttShadow = 0;
  static uint8_t buttTimeout = 0;

  lineSensorLocations lineLocation = LSL_RIGHT_FRONT;

  if(stateNum == MEST_LOAD_CENTER_RINGS)
    lineLocation = LSL_CENTER_FRONT;

  buttShadow |= buttMan.getButtons();

  if(!buttonsDetected && (buttShadow & 0x03 == 0x01))
    rotation = -8;
  else if(!buttonsDetected && (buttShadow & 0x03 == 0x02))
    rotation = 8;

  // Keep going if it has been half a second since a single button was pressed
  if (buttShadow && (buttTimeout < timeBeforeButtonIgnore)) buttTimeout++;

  /* Buttons Detected */
  if (!buttonsDetected && (((buttShadow & 0x03) == 0x03) || (buttTimeout >= timeBeforeButtonIgnore)) && (distanceToFront < 1)) {
    timeOut = micros();
    buttonsDetected = true;
    wheels.updateCommand(0,0,0);
  }
  else if (!buttonsDetected && rotation != 0) {
    wheels.updateCommand(0 ,0,rotation);
  }
  else if (!buttonsDetected) {
    //Use the single front sensor to follow the line so there is not rotation
    //This assumes we are rotationally aligned when entering loadRings
    FollowLineSingle(4,true,lineLocation);
  }

  if (buttonsDetected) {
    switch(stateNum) {
      case MEST_LOAD_LR_RINGS:
        /* Command left and right Top Servos to Loading Position */
        arm.commandPickupServo(PU_LEFT, PS_GRAB);
        arm.commandPickupServo(PU_RIGHT, PS_GRAB);
        stateNum = MEST_BACKUP_ONE;
        break;
      case MEST_LOAD_CENTER_RINGS:
        /* Command center top servo to Loading Position */
        arm.commandPickupServo(PU_CENTER, PS_GRAB);
        stateNum = MEST_BACKUP_TWO;
        break;
      case MEST_SCORE:
        arm.commandPickupServo(PU_LEFT, PS_LETGO);
        arm.commandPickupServo(PU_RIGHT, PS_LETGO);
        arm.commandPickupServo(PU_CENTER, PS_LETGO);
        stateNum = MEST_BACKUP_THREE;
        break;
      default:
        break;
  }
  /* We waited long enough, change state to back up */
  if (micros() - timeOut > ringLoadTime) {
    currentState = (state) &MainExecMachine::backUp;
    //Reset all the statics for next time function is run
    buttShadow = 0;
    buttonsDetected = false;
    buttTimeout = 0;
    if (stateNum < MEST_BACKUP_THREE) desiredHeading = scoreHeading;
    else desiredHeading = loadHeading;
  }

    else if (buttShadow & 0x3 == 0x3){
      if (stateNum < MEST_BACKUP_THREE) {
        loadHeading = mag.getFiltHead();
      }
      else {
        scoreHeading = mag.getFiltHead();
      }
    }
  }
}

void MainExecMachine::backUp(bool firstTime) {
  bool exit = 0;

  /* if we just transistioned to this state*/


  switch (stateNum) {
    case MEST_BACKUP_ONE:
      wheels.updateCommand(-2, 0, 0);
      FollowLine(0, -2, LSP_RIGHT);
      if (lineManager.getLineDriveCommand(LSP_BACK).valid ||
          ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 5)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_SHIFT_FOR_CENTER;
        currentState = (state) &MainExecMachine::shiftForCenter;
      }
      break;
    case MEST_BACKUP_TWO:
      wheels.updateCommand(-20, 0, 0);
      if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_RAISE_ARM;
        currentState = (state) &MainExecMachine::mainArm;
        arm.commandSwingArm(SA_UP);
      }
      break;
    case MEST_BACKUP_THREE:
      wheels.updateCommand(-20, 0, 0);
      if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_LOWER_ARM;
        currentState = (state) &MainExecMachine::mainArm;
        arm.commandSwingArm(SA_DOWN);
      }
      break;
    default:
      break;
  }
}

void MainExecMachine::mainArm(bool firstTime) {
  static bool setTimeout = true;

  if (setTimeout) {
    if (stateNum == MEST_RAISE_ARM)
      arm.commandSwingArm(SA_UP);
    else
      arm.commandSwingArm(SA_DOWN);
    timeOut = micros();
    setTimeout = false;
  }
  else if (micros() - timeOut > armSwingTime){
     if (stateNum == MEST_RAISE_ARM) {
       stateNum = MEST_FLIP_ONE;
     }
     else {
       stateNum = MEST_FLIP_TWO;
     }
     currentState = (state) &MainExecMachine::flipABitch;
     setTimeout = true;
  }

}

void MainExecMachine::shiftForCenter(bool firstTime) {
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
                currentState = (state) &MainExecMachine::loadRings;
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

void MainExecMachine::flipABitch(bool firstTime) {
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(desiredHeading);
  // once the desired heading is reached
  //   transition to findCenterLin
  rotationSpeed =  getToHeadingDirection(desiredHeading,true);

  if(abs(rotationSpeed) < 1 || abs(delta) < 3){
    if(lineUpCount++ > 10){
        if (stateNum == MEST_FLIP_ONE)
          stateNum = MEST_FIND_CENTER_LINE_ONE;
        else if (stateNum == MEST_FLIP_TWO)
          stateNum = MEST_FIND_CENTER_LINE_TWO;
        currentState = (state) &MainExecMachine::findCenterLine;
        lineUpCount = 0;
    }
  }
  else {
        point_t point= {3.5,0};
        wheels.updateCommand(0,0,rotationSpeed,point);
        lineUpCount = 0;
  }
}

void MainExecMachine::findCenterLine(bool firstTime) {

  float forwardSpeed = 15;
  float sidewaysSpeed = 5;
  float rotSpeed = 4;
  static bool goRight = true;
  static bool first = true;

  float rightUltra = ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float leftUltra = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();

  // If first time then set goRight to the short way to prevent zig-zag
  if (first) {
    first = false;
    if ((leftUltra + 7) < (rightUltra + 1.5)) goRight = true;
    else goRight = false;
  }

  // Stop going forward if getting too close to wall
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() < 24)
    forwardSpeed = 0;

  // Check if we need to switch the sideways direction
  if(rightUltra < 12) goRight = false;
  else if (leftUltra < 12) goRight = true;

  // When the right pair is valid, exit criteria has been met
  if(lineManager.getLineDriveCommand(LSP_RIGHT).valid) {
    first = true;

    switch (stateNum) {
      case MEST_FIND_CENTER_LINE_ONE:
        stateNum = MEST_HAUL_TOSCORE;
        break;
      case MEST_FIND_CENTER_LINE_TWO:
        stateNum = MEST_HAUL_TOLOAD;
        break;
      default:
        break;
    }
    currentState = (state) &MainExecMachine::haulAss;
    wheels.updateCommand(0,0,0);
  }

  // If the right front sensor is valid, pivot to get back sensor on
  else if(lineManager.getSingleCommand(LSL_RIGHT_FRONT).valid) {
    if (goRight) wheels.updateCommand(forwardSpeed, 0, -rotSpeed, sensorCenters[LSL_RIGHT_FRONT]);
    else wheels.updateCommand(forwardSpeed, 0, rotSpeed, sensorCenters[LSL_RIGHT_FRONT]);
  }

  // If the right back sensor is valid, pivot to get front sensor on
  else if(lineManager.getSingleCommand(LSL_RIGHT_BACK).valid) {
    if (goRight) wheels.updateCommand(forwardSpeed, 0, rotSpeed, sensorCenters[LSL_RIGHT_BACK]);
    else wheels.updateCommand(forwardSpeed, 0, -rotSpeed, sensorCenters[LSL_RIGHT_BACK]);
  }

  // Else drive forward and sideways
  else {
    if (goRight) wheels.updateCommand(forwardSpeed, sidewaysSpeed, 0);
    else wheels.updateCommand(forwardSpeed, -sidewaysSpeed, 0);
  }
}

void MainExecMachine::haulAss(bool firstTime) {
  static int8_t sideSpeed;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float rot = getToHeading(desiredHeading);
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  sideSpeed = 2*(rightError - leftError);
  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    wheels.updateCommand(0, sideSpeed, rot);
  }
  else if (frontDist > 15) {
    forwardSpeed = (frontDist-15) + 2;
    forwardSpeed = forwardSpeed > 50 ? 50 : forwardSpeed;
    forwardSpeed = forwardSpeed < 2 ? 2 : forwardSpeed;
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
  }
  else {
    wheels.updateCommand(0, 0, 0);
    if (lineManager.getLineDriveCommand(LSP_RIGHT).valid) {
      if (stateNum == MEST_HAUL_TOSCORE)
        stateNum = MEST_SCORE;
      else if (stateNum == MEST_HAUL_TOLOAD)
        stateNum = MEST_LOAD_LR_RINGS;
      currentState = (state) &MainExecMachine::loadRings;
    }
  }
}

void MainExecMachine::DebugOutput(HardwareSerial * serialPort){
  serialPort->print("stateNum: ");
  serialPort->println(stateNum);
}

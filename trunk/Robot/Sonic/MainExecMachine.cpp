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
  static uint8_t forwardSpeed = 3;
  float distanceToFront = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  /* Move Forward Till we hit Buttons */
  static uint8_t buttShadow = 0;
  float sideSpeed = 0;
  float leftError = 8.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  if((stateNum != MEST_LOAD_CENTER_RINGS) && (stateNum != MEST_BACKUP_TWO))
    sideSpeed = 2*(rightError - leftError);

  buttShadow |= buttMan.getButtons();

  if(!buttonsDetected && (buttShadow & 0x03))
    forwardSpeed += 2;

  /* Buttons Detected */
  if (!buttonsDetected && ((buttShadow & 0x03) == 0x03) && (distanceToFront < 1)) {
    timeOut = micros();
    buttonsDetected = true;
    wheels.updateCommand(0,0,0);
  }
  else if (!buttonsDetected) {
    wheels.updateCommand(forwardSpeed,sideSpeed,0);
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
        stateNum = MEST_BACKUP_THREE;
        break;
      default:
       // Still Waiting, determin desired heading
        currentHeading = mag.getFiltHead();
        desiredHeading = currentHeading - 180;
        break;
  }
    /* We waited long enough, change state to back up */
    if (micros() - timeOut > ringLoadTime) {
      currentState = (state) &MainExecMachine::backUp;
      buttShadow = 0;
      forwardSpeed = 3;
      buttonsDetected = false;
    }
  }
}

void MainExecMachine::backUp(bool firstTime) {
  bool exit = 0;
  /* if we just transistioned to this state*/

  wheels.updateCommand(-3,0,0);

  switch (stateNum) {
    case MEST_BACKUP_ONE:
      if (lineManager.getLineDriveCommand(LSP_BACK).valid ||
          ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 7.5)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_SHIFT_FOR_CENTER;
        currentState = (state) &MainExecMachine::shiftForCenter;
      }
      break;
    case MEST_BACKUP_TWO:
      if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_FLIP_ONE;
        currentState = (state) &MainExecMachine::flipABitch;
      }
      break;
    case MEST_BACKUP_THREE:
      if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 16)
      {
        wheels.updateCommand(0,0,0);
        stateNum = MEST_FLIP_TWO;
        currentState = (state) &MainExecMachine::flipABitch;
      }
      break;
    default:
      break;
  }
}

void MainExecMachine::shiftForCenter(bool firstTime) {
  static bool centerSensorCount;
  static bool rightSensorCount;

  wheels.updateCommand(0,3,0);

  if(lineManager.getLineDriveCommand(LSP_CENTER).valid) {
    centerSensorCount = true;
  }

  if(centerSensorCount && lineManager.getLineDriveCommand(LSP_RIGHT).valid) {
    rightSensorCount = true;
  }

  if (centerSensorCount && rightSensorCount &&
      lineManager.getLineDriveCommand(LSP_CENTER).valid) {
    wheels.updateCommand(0,0,0);
    stateNum = MEST_LOAD_CENTER_RINGS;
    currentState = (state) &MainExecMachine::loadRings;
    centerSensorCount = false;
    rightSensorCount = false;
  }
}

void MainExecMachine::flipABitch(bool firstTime) {
  float delta;
  // once the desired heading is reached
  //   transition to findCenterLin
  delta = getToHeading(desiredHeading);

  if(delta > 2)
    delta = 2;
  else if (delta < -2)
    delta = -2;

  if(delta == 0){
    if (stateNum == MEST_FLIP_ONE)
      stateNum = MEST_FIND_CENTER_LINE_ONE;
    else if (stateNum == MEST_FLIP_TWO)
      stateNum = MEST_FIND_CENTER_LINE_TWO;
    currentState = (state) &MainExecMachine::findCenterLine;
  }
  else {
    wheels.updateCommand(0,0,delta);
  }
}

void MainExecMachine::findCenterLine(bool firstTime) {
  int8_t forwardSpeed = 15;

  // Check Ultrasonic Sensors and Magnetometer to move to the center line
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() < 18)
    forwardSpeed = 0;


  if(ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue() >
     (ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue() + 9)) {
       wheels.updateCommand(forwardSpeed,5,0);
     }
  else
    wheels.updateCommand(forwardSpeed, -5,0);

  // If found centerline on Right Line Sensors, transition to haulAss
  if (lineManager.getLineDriveCommand(LSP_RIGHT).valid) {

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
}

void MainExecMachine::haulAss(bool firstTime) {
  static int8_t sideSpeed;
  float leftError = 8.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float rot = getToHeading(desiredHeading);
  float forwardSpeed;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  sideSpeed = 2*(rightError - leftError);
  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    wheels.updateCommand(forwardSpeed, sideSpeed, rot);
  }
  else if (frontDist > 10) {
    FollowLine(0,50 * frontDist / 95.0 ,  LSP_RIGHT);
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

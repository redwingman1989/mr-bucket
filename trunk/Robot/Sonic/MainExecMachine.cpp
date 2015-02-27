#include "MainExecMachine.h"
#include "Globals.h"

MainExecMachine::MainExecMachine() {
  currentState = (state) &MainExecMachine::loadLandR;
  stateNum = MEST_LOAD_LR_RINGS;
}


void MainExecMachine::load(bool firstTime) {
  static bool buttonsDetected = 0;
  float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();
  /* Move Forward Till we hit Buttons */

  /* Buttons Detected */
  if (!buttonsDetected && ((buttMan.getButtons() & 0x03) && distanceToFront < 1)) {
    timeOut = micros();
    buttonsDetected = true;
    wheels.KillMotors();
  }
  else {
    wheels.updateCommand(15,0,0);
  }

  if (buttonsDetected) {
    switch(stateNum) {
      case MEST_LOAD_LR_RINGS:
        /* Command left and right Top Servos to Loading Position */
        arm.commandPickupServo(PU_LEFT, pickupStates_t.PS_GRAB);
        arm.commandPickupServo(PU_RIGHT, pickupStates_t.PS_GRAB);
        StateNum = MEST_BACKUP_ONE;
        break;
      case MEST_LOAD_CENTER_RINGS:
        /* Command center top servo to Loading Position */
        arm.commandPickupServo(PU_CENTER, pickupStates_t.PS_GRAB);
        StateNum = MEST_BACKUP_TWO;

      default:
        // Still Waiting, determin desired heading
        currentHeading = mag->getFiltHead();
        desiredHeading = currentHeading - 180;
        break;
    }
  }

  /* We waited long enough, change state to back up */
  if (micros() - timeOut > ringLoadTime) {
    currentState = (state) &MainExecMachine::backUp;
    buttonsDetected = false;
  }
}

void MainExecMachine::backUp(bool firstTime) {
  bool exit = 0;

  /* if we just transistioned to this state*/

  wheels.updateCommand(-15,0,0);

  switch (stateNum) {
    case MEST_BACKUP_ONE:
      if (lineManager.getLineDriveCommand(lineSensorPairs.LSP_BACK).valid ||
          ultraSonicMgr.getSensor(FRONT)->calculateDistance() > 7.5)
      {
        wheels.KillMotors();
        stateNum = MEST_SHIFT_FOR_CENTER;
        currentState = (state) &MainExecMachine::shiftForCenter;
      }
      break;
    case MEST_BACKUP_TWO:
      if (ultraSonicMgr.getSensor(FRONT)->calculateDistance() > 16)
      {
        wheels.KillMotors();
        stateNum = MEST_FLIP_ONE;
        currentState = (state) &MainExecMachine::flipABitch;
      }
      break;
    case MEST_BACKUP_THREE:
      if (ultraSonicMgr.getSensor(FRONT)->calculateDistance() > 16)
      {
        wheels.KillMotors();
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

  wheels.updateCommand(0,5,0);

  if(lineManager.getLineDriveCommand(lineSensorPairs.LSP_CENTER).valid) {
    centerSensorCount = true;
  }

  if(centerSensorCount && lineManager.getLineDriveCommand(lineSensorPairs.LSP_RIGHT).valid)) {
    rightSensorCount = true;
  }

  if (centerSensorCount && rightSensorCount &&
      lineManager.getLineDriveCommand(lineSensorPairs.LSP_CENTER) {
    wheels.KillMotors();
    stateNum = MEST_LOAD_CENTER_RINGS;
    currentState = (state) &MainExecMachine::load;
    centerSensorCount = false;
    rightSensorCount = false;
  }
}

void MainExecMachine::flipABitch(bool firstTime) {
  //

  // rotate till current heading is within tollerance of desired heading

  // once the desired heading is reached
  //   transition to findCenterLin
}

void MainExecMachine::findCenterLine(bool firstTime) {
  // Check Ultrasonic Sensors and Magnetometer to move to the center line

  // If found centerline on Right Line Sensors, transition to haulAss
}

void MainExecMachine::haulAss() {

}

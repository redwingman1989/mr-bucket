#include "driveAlgorithms.h"
#include "Globals.h"
#include "stateMachineCommon.h"
#include "driveAlgorithms.h"

// Used to drive down the course and stay on the line
// Returns true when completed (<15 inches on front and on line)
bool findCenterLine(bool first, float forwardSpeed, float sidewaysSpeed, float rotSpeed) {
  static bool goRight = true;

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

  if(lineManager.getLineDriveCommand(LSP_RIGHT).valid)
    return true;

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

  return false;
}

/****************************************
 * Function:    loadRingsButtonDetection
 * Inputs:      lineSensorLocations lineLocation, loadRingsSharedStaticData_t * staticData
 * Outputs:     void
 * Description: This function looks at what is happening with the buttons.
 *                Later the Load Rings State Machine will determine what to do with the arms.
 *****************************************/
void loadRingsButtonDetection(lineSensorLocations lineLocation,
                              loadRingsSharedStaticData_t * staticData) {
  /* Static Variable(s) */
  int8_t rotation = 0;
  /* Local variable(s) */
  float distanceToFront = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  /* Constant Variable(s) */
  const int timeBeforeButtonIgnore = 50;

  /* Update the static data structure with the latest buttons reading */
  staticData->staticButtShadow |= buttMan.getButtons();

  if(!staticData->staticButtonsDetected && (staticData->staticButtShadow & 0x03 == 0x01))
    rotation = -8;
  else if(!staticData->staticButtonsDetected && (staticData->staticButtShadow & 0x03 == 0x02))
    rotation = 8;

  // Keep going if it has been half a second since a single button was pressed
  if (staticData->staticButtShadow && (staticData->staticButtTimeout < timeBeforeButtonIgnore)) staticData->staticButtTimeout++;

  /* Buttons Detected */
  if (!staticData->staticButtonsDetected && (((staticData->staticButtShadow & 0x03) == 0x03) ||
      (staticData->staticButtTimeout >= timeBeforeButtonIgnore)) && (distanceToFront < 1)) {
    //staticData->staticStateTimeout = micros();
    staticData->staticButtonsDetected = true;
    wheels.updateCommand(0,0,0);
  }
  else if (!staticData->staticButtonsDetected && rotation != 0) {
    wheels.updateCommand(0 ,0,rotation);
  }
  else if (!staticData->staticButtonsDetected &&
            distanceToFront >= 1              &&
            micros() - staticData->staticStateTimeout >= loadRingsButtonTimeout) {
    staticData->staticButtonTimeoutFlag = true;
  }
  else if (!staticData->staticButtonsDetected) {
    //Use the single front sensor to follow the line so there is not rotation
    //This assumes we are rotationally aligned when entering loadRings
    FollowLineSingle(2,true,lineLocation);
  }
}


/****************************************
 * Function:    loadRingsTimeOutCheck
 * Inputs:      loadRingsSharedStaticData_t * staticData
 * Outputs:     bool stateTimedOut
 * Description: This function checks to make sure the calling Load Rings
 *                State Machine hasn't timed out.
 *****************************************/
bool loadRingsTimeOutCheck (loadRingsSharedStaticData_t * staticData) {
  bool stateTimedOut = false;

  /* Buttons Detected flag will be set after a timeout period. See loadRingsButtonDetection() */
  if (!staticData->staticButtonsDetected) {
    if (micros() - staticData->staticStateTimeout > ringLoadTime) {
      /* Set flag eventually returned to indicate that the state timed out */
      stateTimedOut = true;
      /* Reset the static data, due to the timeout */
      staticData->staticButtShadow = 0;
      staticData->staticButtonsDetected = false;
      staticData->staticButtTimeout = 0;
    }
  }
  return stateTimedOut;
}


void clearStaticData (loadRingsSharedStaticData_t * staticData) {
  staticData->staticButtonsDetected = false;
  staticData->staticButtonTimeoutFlag = false;
  staticData->staticButtShadow = 0;
  staticData->staticButtTimeout = 0;
  staticData->staticStateTimeout = micros();
}

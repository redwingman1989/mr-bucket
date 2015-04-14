#include "driveAlgorithms.h"
#include "Globals.h"
#include "stateMachineCommon.h"
#include "driveAlgorithms.h"

// Used to drive down the course and stay on the line
// Returns true when completed (<15 inches on front and on line)
bool findCenterLine(bool first, float forwardSpeed, float sidewaysSpeed, float rotSpeed) {
  static bool goRight = true;
  static bool goRightShadow = true;
  static float threshHold = 12.0;

  float rightUltra = ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float leftUltra = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();

  // If first time then set goRight to the short way to prevent zig-zag
  if (first) {
    threshHold = 12.0;
    first = false;
    if ((leftUltra + 8.5) < (rightUltra + 1.5)) goRight = true;
    else goRight = false;
  }

  // Stop going forward if getting too close to wall
  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() < 24)
    forwardSpeed = 0;

  // Check if we need to switch the sideways direction
  if(rightUltra < threshHold) goRight = false;
  else if (leftUltra < threshHold) goRight = true;

  if (goRight != goRightShadow) {
    if(threshHold > 4.0) {
     threshHold = threshHold - 3.0;
    }
    goRightShadow = goRight;
  }

  if(lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    threshHold = 12.0;
    return true;
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

  return false;
}

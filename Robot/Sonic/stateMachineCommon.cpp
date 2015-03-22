/* Source file for common State Machine functionality */

// Used to drive down the course and stay on the line
// Returns true when completed (<15 inches on front and on line)
#include "Globals.h"
#include "driveAlgorithms.h"

bool haulAss(bool first) {
  static int8_t sideSpeed;
  float leftError = 4.5 + ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float rightError = 1.5 + ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  float forwardSpeed = 0;
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();

  sideSpeed = 2*(rightError - leftError);


  if(! lineManager.getLineDriveCommand(LSP_RIGHT).valid){
    //Replace with call to find centerline
    wheels.updateCommand(0, sideSpeed, 0);
  }

  else if (frontDist > 15) {
    forwardSpeed = (frontDist-15) + 2;
    forwardSpeed = forwardSpeed > 50 ? 50 : forwardSpeed;
    forwardSpeed = forwardSpeed < 2 ? 2 : forwardSpeed;
    FollowLine(0, forwardSpeed ,  LSP_RIGHT);
  }

  else {
    wheels.updateCommand(0, 0, 0);
    return true;
  }

  return false;
}

bool findCenterLine(bool first) {
  float forwardSpeed = 15;
  float sidewaysSpeed = 5;
  float rotSpeed = 4;
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

bool flipABitch(float desiredHeading) {
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(desiredHeading);
  // once the desired heading is reached
  //   transition to findCenterLin
  rotationSpeed =  getToHeadingDirection(desiredHeading,true);

  if(abs(rotationSpeed) < 1 || abs(delta) < 3){
    if(lineUpCount++ > 10){
        lineUpCount = 0;
        return true;
    }
  }
  else {
        point_t point= {3.5,0};
        wheels.updateCommand(0,0,rotationSpeed,point);
        lineUpCount = 0;
  }

  return false;
}

#include "DoublePointsStateMachine.h"
#include "driveAlgorithms.h"

DpExecMachine::DpExecMachine() {
  currentState = (state) &DpExecMachine::backUpInitial;
}

bool DpExecMachine::RunTick(void) {
  this->runCurrentState();
  return this->doneDoublingPoints;
}


void DpExecMachine::backUpInitial(bool first) {
  static unsigned long startTime;
  float backSpeed = 0;
  float sideSpeed = 0;

  if (first) startTime = micros();

  if (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() < 10) backSpeed = -10;
  if (ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue() < 10) sideSpeed = 10;
  if (ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue() < 10) sideSpeed = -10;

  wheels.updateCommand(backSpeed, sideSpeed, 0);

  if ((micros() - startTime > 3000000) || ((abs(backSpeed) > 5) && (abs(sideSpeed) < 5))) {
    currentState = (state) &DpExecMachine::rotateToHeading;
    calcCenterPoleHeading();
  }
}


void DpExecMachine::rotateToHeading(bool first) {
  float rotationSpeed;
  static int lineUpCount = 0;
  float delta = getDeltaHeading(centerPoleHeading);
  // once the desired heading is reached
  //   transition to findCenterLin
  rotationSpeed =  getToHeading(centerPoleHeading);

  if(abs(delta) < 3){
    if(lineUpCount++ >= 0){
        lineUpCount = 0;
        currentState = (state) &DpExecMachine::backUpToWall;
    }
  }
  else {
        point_t point= {2,0};
        wheels.updateCommand(0,0,rotationSpeed/2.0,point);
        lineUpCount = 0;
  }
}


void DpExecMachine::backUpToWall(bool first) {
  const float minSpeed = 0;   // in motor command units
  const float maxSpeed = 38;  // in motor command units
  const float maxDist = 34.0; // in inches
  const float minDist = 12.0; // in inches
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float lastFront = 0;
  static uint8_t limitCounter = 0;

  float backSpeed = scaleDistanceToSpeedCmd(frontDist,
                                           maxDist,
                                           minDist,
                                           maxSpeed,
                                           minSpeed);
  backSpeed = 40 - backSpeed;
  wheels.updateCommand(-backSpeed, 0, 0);

  limitCounter++;

  if (limitCounter >= 50) {
    limitCounter = 0;

    if (frontDist - lastFront < 0.375)
      currentState = (state) &DpExecMachine::shiftForCenterPost;

    lastFront = frontDist;
  }
}


void DpExecMachine::shiftForCenterPost( bool first) {
  static uint8_t tickCount = 0;

  if(findCenterLine(first, -1, 15, 0)) {
    if(lineUpOneLine(LSP_RIGHT)) {
      if (++tickCount > 50)
        currentState = (state) &DpExecMachine::deployTheSecretWeapon;
    }
    else
      tickCount = 0;
  }
}

void DpExecMachine::deployTheSecretWeapon(bool first) {
  static uint32_t startTime;
  bool timedOut = false;

  if(first) {
    startTime = micros();
    arm.commandDoublePointServo(DP_DROP);
  }

  else if (micros() - startTime > 2000000) {
    timedOut = true;
  }

  if (timedOut) {
      currentState = (state) &DpExecMachine::avoidHovering;
  }
}

void DpExecMachine::avoidHovering(bool first) {
  static uint32_t startTime;
  bool timedOut = false;

  if(first) {
    startTime = micros();
    wheels.updateCommand(4,0,0);
  }

  else if (micros() - startTime > 20000000) {
    timedOut = true;
  }

  if (timedOut) {
      wheels.updateCommand(0,0,0);
      doneDoublingPoints = true;
  }



}



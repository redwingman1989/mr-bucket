#include "DoublePointsStateMachine.h"
#include "driveAlgorithms.h"
#include "Globals.h"

DpExecMachine::DpExecMachine() {
  currentState = (state) &DpExecMachine::backUpInitial;
  DirectionToDPRight =  false;
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

  if ((micros() - startTime > 3000000) || ((abs(backSpeed) < 5) && (abs(sideSpeed) < 5))) {
    currentState = (state) &DpExecMachine::rotateToHeading;
    calcCenterPoleHeading();
  }
}


void DpExecMachine::rotateToHeading(bool first) {
  float rotationSpeed = 0;
  static int lineUpCount = 0;
  static float leftDist = 0 , rightDist = 0;
  float delta = getDeltaHeading(centerPoleHeading);

  arm.commandSwingArm(SA_DOWN);

  if(abs(delta) < 5){
    leftDist = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
    rightDist = ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
    if(lineUpCount++ >= 10){
      lineUpCount = 0;
      DirectionToDPRight = leftDist - 3.5 < rightDist;
      currentState = (state) &DpExecMachine::backUpToWall;
    }
  }
  else {
    rotationSpeed =  getToHeading(centerPoleHeading);
    lineUpCount = 0;
    leftDist = rightDist = 0;
  }
  wheels.updateCommand(0,0,rotationSpeed);
}


void DpExecMachine::backUpToWall(bool first) {
  const float minSpeed = 0;   // in motor command units
  const float maxSpeed = 28;  // in motor command units
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
  backSpeed = 30 - backSpeed;
  wheels.updateCommand(-backSpeed,0,0);

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
  static float speedBackIntegral = 0;
  static unsigned long stateStartTime;

  if (first) stateStartTime = micros();

  if(lineManager.getLineDriveCommand(LSP_RIGHT).valid) {
    if(FollowLineSingle(-4, true, LSL_RIGHT_FRONT, first)) {
      if (++tickCount > 50)
        currentState = (state) &DpExecMachine::waitForLastSeconds;
    }
    else
      tickCount = 0;
  }
  else {
    if ( micros() - stateStartTime > 5000000 ) {
        DirectionToDPRight = !DirectionToDPRight;
        stateStartTime = micros();
    }
    if(DirectionToDPRight){
         wheels.updateCommand(-1,15,0);
    }
    else{
        wheels.updateCommand(-1,-15,0);
    }
    tickCount = 0;
  }
}

void DpExecMachine::waitForLastSeconds(bool first) {
  float frontDist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float lastFront = 0;
  static uint8_t limitCounter = 0;

  if (micros() - runTimeStart > 173000000) {
    FollowLineSingle(-2, true, LSL_RIGHT_FRONT, first);
    limitCounter++;
    if (limitCounter >= 50) {
      limitCounter = 0;
      if (frontDist - lastFront < 0.375)
        currentState = (state) &DpExecMachine::deployTheSecretWeapon;
      lastFront = frontDist;
    }
  }
  else if (frontDist > 26) {
    FollowLineSingle(2, true, LSL_RIGHT_FRONT, first);
  }
  else {
    wheels.updateCommand(0,0,0);
  }
}

void DpExecMachine::deployTheSecretWeapon(bool first) {
  static uint32_t startTime;
  bool timedOut = false;

  if(first) {
    startTime = micros();
    arm.commandDoublePointServo(DP_DROP);
  }

  else if (micros() - startTime > 1000000) {
    timedOut = true;
  }

  if (timedOut) {
      currentState = (state) &DpExecMachine::doubleTap;
  }
}

void DpExecMachine::doubleTap(bool first) {
  static uint32_t startTime;
  static bool backup = false;

  if (first)
    startTime = micros();

  if(!backup && ((micros() - startTime) < 500000)) {
    FollowLineSingle(4, true, LSL_RIGHT_FRONT, first);
  }
  else if (!backup) {
    backup = true;
    startTime = micros();
  }
  else if (backup && (micros() - startTime < 750000)) {
    FollowLineSingle(-5, true, LSL_RIGHT_FRONT, first);
  }

  else {
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

  else if (micros() - startTime > 7000000) {
    timedOut = true;
  }

  if (timedOut) {
      wheels.updateCommand(0,0,0);
      doneDoublingPoints = true;
  }



}



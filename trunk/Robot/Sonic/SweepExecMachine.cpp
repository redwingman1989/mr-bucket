#include "SweepExecMachine.h"
#include "Globals.h"
#include "driveAlgorithms.h"
#include "stateMachineCommon.h"

SweepExecMachine::SweepExecMachine(Zone z) {
  /* Initalize State Arrays */
  const state lzStateArray[] = {
    (state) &SweepExecMachine::lz_backUpInitial,
    (state) &SweepExecMachine::lz_shiftLeftToWall,
    (state) &SweepExecMachine::lz_driveForward,
    (state) &SweepExecMachine::lz_shiftRightToWall,
    (state) &SweepExecMachine::lz_backOffLeft,
    (state) &SweepExecMachine::lz_backUpToAlign,
    (state) &SweepExecMachine::lz_reAlign
  };

  const state szStateArray[] = {
    (state) &SweepExecMachine::sz_backUpInitial,
    (state) &SweepExecMachine::sz_shiftLeftToWall,
    (state) &SweepExecMachine::sz_driveForward,
    (state) &SweepExecMachine::sz_shiftRightToWall,
    (state) &SweepExecMachine::sz_backOffLeft,
    (state) &SweepExecMachine::sz_backUpToAlign,
    (state) &SweepExecMachine::sz_reAlign,
  };

  switch (z) {
    case Z_LOAD:
      currentState = (state) &SweepExecMachine::lz_backUpInitial;
      break;
    case Z_SCORE:
    default:
      szState = SZ_BACK_UP_INITIAL;
      currentState = (state) &SweepExecMachine::sz_backUpInitial;
      break;
  }
}

bool SweepExecMachine::RunTick() {
  runCurrentState();
  return doneSweeping;
}

/*--- Loading Zone Function Definitions ---*/
void SweepExecMachine::lz_backUpInitial(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;

  if (firstTime) {
    startTime = micros();
    doneSweeping = false;
    firstTime = false;
  }

  distanceReached = (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 5) ? true : false;
  timedOut = (micros() - startTime > 3000000) ? true : false;

  /* If we are further than 5 inchecs from the wall, or have been backing up for 3 seconds, advance to next state */
  if(distanceReached || timedOut) {
    /* reset Static Variables */
    firstTime = true;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_shiftLeftToWall;
  }
  /* Continue Backing up */
  else {
    wheels.updateCommand(-10,0,0);
  }
}

void SweepExecMachine::lz_shiftLeftToWall(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float distLeft = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float distRight = ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  static float prevDistRight = 0.0;
  const float minSpeed = 3; // in motor command units
  const float maxSpeed = 50;  // in motor command units
  const float maxDist = 48.0; // in inches
  const float minDist = 3.0;  // in inches
  const float distDeadBand = 0.25; // 0.5 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
    /* Command Arms to Desired Position */
    arm.commandPickupServo(PU_LEFT, PS_GRAB);
    arm.commandPickupServo(PU_CENTER, PS_GRAB);
    arm.commandPickupServo(PU_RIGHT, PS_GRAB);
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance on the rightsensor is the same within the tollerence */
  if ((iter++ % 50) == 0) {
    distanceReached = (abs(distRight - prevDistRight) < distDeadBand);
    prevDistRight = distRight;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 5000000);

  /* If against the wall or timedOut after 5 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_driveForward;
  }
  /* Continue moving Left */
  else {
    speed = scaleDistanceToSpeedCmd(distLeft,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    wheels.updateCommand(0,-speed,0);
  }
}

void SweepExecMachine::lz_driveForward(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float dist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float prevDist = 0.0;
  const float minSpeed = 3; // in motor command units
  const float maxSpeed = 100;  // in motor command units
  const float maxDist = 96.0; // in inches
  const float minDist = 3.0;  // in inches
  const float distDeadBand = 0.25; // 0.5 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance isnt changing,
   * or if one button has been depressed and we are within an
   * inch from the wall
   */
  if ((iter++ % 50) == 0) {
    distanceReached = ((abs(dist - prevDist) < distDeadBand) ||
                       ((dist < 1.0) && (buttMan.getButtons() & 0x03)));
    prevDist = dist;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 5000000);

  /* If against the wall or timedOut after 5 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_shiftRightToWall;
  }
  /* Continue moving forward */
  else {
    speed = scaleDistanceToSpeedCmd(dist,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    wheels.updateCommand(speed,-1,0);
  }
}

void SweepExecMachine::lz_shiftRightToWall(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  const float speed = 10.0;
  float distLeft = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  static float prevDistLeft = 0.0;
  const float distDeadBand = 0.375; // 0.75 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance on the rightsensor is the same within the tollerence */
  if ((iter++ % 50) == 0) {
    distanceReached = (abs(distLeft - prevDistLeft) < distDeadBand);
    prevDistLeft = distLeft;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 6000000);

  /* If against the wall or timedOut after 7 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_backOffLeft;
  }
  /* Continue moving Right */
  else {
    wheels.updateCommand(1,speed,0);
  }
}

void SweepExecMachine::lz_backOffLeft(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  const float speed = 1.5;

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 250000);

  /* If timedOut after 0.5 second, transition states */
  if (timedOut) {
    /* reset Static Variables */
    firstTime = true;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_backUpToAlign;
  }
  /* Continue moving Right */
  else {
    wheels.updateCommand(0,-speed,0);
  }
}

void SweepExecMachine::lz_backUpToAlign(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float dist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float prevDist = 0.0;
  const float minSpeed = 3; // in motor command units
  const float maxSpeed = 15;  // in motor command units
  const float maxDist = 15.0; // in inches
  const float minDist = 1.0;  // in inches
  const float distDeadBand = 0.375; // 0.75 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
    /* Command Arms to Desired Position */
    arm.commandPickupServo(PU_LEFT, PS_LETGO);
    arm.commandPickupServo(PU_CENTER, PS_LETGO);
    arm.commandPickupServo(PU_RIGHT, PS_LETGO);
  }

  if ((iter++ % 50) == 0) {
    distanceReached = ((abs(dist - prevDist) < distDeadBand) ||
                       (dist > maxDist));
    prevDist = dist;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 4000000);

  /* If distance reached or timedOut after 4 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::lz_reAlign;
  }
  /* Continue moving forward */
  else {
    speed = scaleDistanceToSpeedCmd(dist,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    /* Invert Speed so it slows down as it gets awa from the wall */
    speed = maxSpeed - speed;
    wheels.updateCommand(-speed,0,0);
  }
}

void SweepExecMachine::lz_reAlign(bool first){
  static bool firstTime = true;

  if (findCenterLine(firstTime, 0, 4, 4)) {
    firstTime = true;
    doneSweeping = true;
    currentState = (state) &SweepExecMachine::lz_backUpInitial;
  }
  else firstTime = false;
}

/*--- Scoring Zone Function Definitions ---*/
void SweepExecMachine::sz_backUpInitial(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;

  if (firstTime) {
    startTime = micros();
    doneSweeping = false;
    firstTime = false;
  }

  distanceReached = (ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue() > 5) ? true : false;
  timedOut = (micros() - startTime > 3000000) ? true : false;

  /* If we are further than 5 inchecs from the wall, or have been backing up for 3 seconds, advance to next state */
  if(distanceReached || timedOut) {
    /* reset Static Variables */
    firstTime = true;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_shiftLeftToWall;
  }
  /* Continue Backing up */
  else {
    wheels.updateCommand(-10,0,0);
  }
}

void SweepExecMachine::sz_shiftLeftToWall(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float distLeft = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  float distRight = ultraSonicMgr.getSensor(RIGHT)->getCalculatedDistanceValue();
  static float prevDistRight = 0.0;
  const float minSpeed = 1.5; // in motor command units
  const float maxSpeed = 20;  // in motor command units
  const float maxDist = 48.0; // in inches
  const float minDist = 3.0;  // in inches
  const float distDeadBand = 0.25; // 0.5 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
    /* Command Arms to Desired Position */
    arm.commandSwingArm(SA_DOWN);
    arm.commandPickupServo(PU_LEFT, PS_LETGO);
    arm.commandPickupServo(PU_CENTER, PS_LETGO);
    arm.commandPickupServo(PU_RIGHT, PS_LETGO);
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance on the rightsensor is the same within the tollerence */
  if ((iter++ % 50) == 0) {
    distanceReached = (abs(distRight - prevDistRight) < distDeadBand);
    prevDistRight = distRight;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 5000000);

  /* If against the wall or timedOut after 5 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_driveForward;
  }
  /* Continue moving Left */
  else {
    speed = scaleDistanceToSpeedCmd(distLeft,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    wheels.updateCommand(0,-speed,0);
  }
}

void SweepExecMachine::sz_driveForward(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float dist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float prevDist = 0.0;
  const float minSpeed = 1.5; // in motor command units
  const float maxSpeed = 40;  // in motor command units
  const float maxDist = 96.0; // in inches
  const float minDist = 1.0;  // in inches
  const float distDeadBand = 0.10; // 0.2 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance isnt changing,
   * or if one button has been depressed and we are within an
   * inch from the wall
   */
  if ((iter++ % 50) == 0) {
    distanceReached = ((abs(dist - prevDist) < distDeadBand) ||
                       ((dist < 1.0) && (buttMan.getButtons() & 0x03)));
    prevDist = dist;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 5000000);

  /* If against the wall or timedOut after 5 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_shiftRightToWall;
  }
  /* Continue moving forward */
  else {
    speed = scaleDistanceToSpeedCmd(dist,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    wheels.updateCommand(speed,-1.5,0);
  }
}

void SweepExecMachine::sz_shiftRightToWall(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  const float speed = 6.0;
  float distLeft = ultraSonicMgr.getSensor(LEFT)->getCalculatedDistanceValue();
  static float prevDistLeft = 0.0;
  const float distDeadBand = 0.25; // 0.5 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if ultrasonic indicate we are against the wall */
  /* every half second, check if the distance on the rightsensor is the same within the tollerence */
  if ((iter++ % 50) == 0) {
    distanceReached = (abs(distLeft - prevDistLeft) < distDeadBand);
    prevDistLeft = distLeft;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 7000000);

  /* If against the wall or timedOut after 7 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_backOffLeft;
  }
  /* Continue moving Right */
  else {
    wheels.updateCommand(1.5,speed,0);
  }
}

void SweepExecMachine::sz_backOffLeft(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  const float speed = 1.5;

  if (firstTime) {
    startTime = micros();
    firstTime = false;
  }

  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 500000);

  /* If timedOut after 0.5 second, transition states */
  if (timedOut) {
    /* reset Static Variables */
    firstTime = true;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_backUpToAlign;
  }
  /* Continue moving Right */
  else {
    wheels.updateCommand(0,-speed,0);
  }
}

void SweepExecMachine::sz_backUpToAlign(bool first){
  static bool firstTime = true;
  static uint32_t startTime;
  bool timedOut = false;
  bool distanceReached = false;
  static uint16_t iter = 0;
  float speed;
  float dist = ultraSonicMgr.getSensor(FRONT)->getCalculatedDistanceValue();
  static float prevDist = 0.0;
  const float minSpeed = 1.5; // in motor command units
  const float maxSpeed = 10;  // in motor command units
  const float maxDist = 15.0; // in inches
  const float minDist = 1.0;  // in inches
  const float distDeadBand = 0.10; // 0.2 in/s

  if (firstTime) {
    startTime = micros();
    firstTime = false;
    /* Command Arms to Desired Position */
    arm.commandSwingArm(SA_UP);
    arm.commandPickupServo(PU_LEFT, PS_GRAB);
    arm.commandPickupServo(PU_CENTER, PS_GRAB);
    arm.commandPickupServo(PU_RIGHT, PS_GRAB);
  }

  if ((iter++ % 50) == 0) {
    distanceReached = ((abs(dist - prevDist) < distDeadBand) ||
                       (dist > maxDist));
    prevDist = dist;
  }
  /* Check if we have timed out in this state */
  timedOut = (micros() - startTime > 4000000);

  /* If distance reached or timedOut after 4 seconds, transition states */
  if (timedOut || distanceReached) {
    /* reset Static Variables */
    firstTime = true;
    iter = 0;
    /* Transition State */
    currentState = (state) &SweepExecMachine::sz_reAlign;
  }
  /* Continue moving forward */
  else {
    speed = scaleDistanceToSpeedCmd(dist,
                                    maxDist,
                                    minDist,
                                    maxSpeed,
                                    minSpeed);
    /* Invert Speed so it slows down as it gets awa from the wall */
    speed = maxSpeed - speed;
    wheels.updateCommand(-speed,0,0);
  }
}

void SweepExecMachine::sz_reAlign(bool first){
  static bool firstTime = true;

  if (findCenterLine(firstTime, 0, 4, 4)) {
    firstTime = true;
    doneSweeping = true;
    currentState = (state) &SweepExecMachine::sz_backUpInitial;
  }
  else firstTime = false;
}

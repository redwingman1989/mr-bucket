#pragma once

#include "Globals.h"

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

void FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum){

    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
    adjustedAngleRad = convertRadToPercent(adjustedAngleRad,max(abs(speedx),abs(speedy)));

    float speedConst = 15 * .25;
    float speedConstCenter = 1;
    float centerThreshhold = .7;
    float speed = 0;
    if(speedy > speedx){
       speed = - 1 *(linePair.offset.x - pairCenters[linePairEnum].x );
       if(abs(speed) > centerThreshhold)
        wheels.updateCommand(speedy ,speedConst ,adjustedAngleRad);
       else
        wheels.updateCommand(speedy ,speed * speedConstCenter ,adjustedAngleRad);
    } else{
       speed = - 1 *(linePair.offset.y - pairCenters[linePairEnum].y );
       if(abs(speed) > centerThreshhold)
        wheels.updateCommand(speedConst , speedx ,adjustedAngleRad);
       else
         wheels.updateCommand(speed * speedConstCenter , speedx ,adjustedAngleRad);
    }
}

float getToHeading(float desiredHeading) {
  float heading = mag.getFiltHead();
  float delta;
  static float lastDelta = 0;
  float deltaDelta;
  float returnVal;

  delta = desiredHeading - heading;
  if (delta > 180)
    delta -= 360;
  if (delta < -180)
    delta += 360;
  deltaDelta = delta - lastDelta;

  if (abs(delta) <= 2 && abs(deltaDelta) < 0.25) {
    returnVal = 0;
  }
  else {
    returnVal = (0.2 * delta);
  }

  lastDelta = delta;

  return returnVal;
}

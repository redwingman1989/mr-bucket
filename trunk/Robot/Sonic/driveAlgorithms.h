#pragma once

#include "Globals.h"

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

void FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum){
    static float xLast = 0;
    static float yLast = 0;

    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
    adjustedAngleRad = convertRadToPercent(adjustedAngleRad,max(abs(speedx),abs(speedy)));
    float xDistance = - 1 *(linePair.offset.x - pairCenters[linePairEnum].x );
    float yDistance = - 1 *(linePair.offset.y - pairCenters[linePairEnum].y );
    float speedConst = 15 * .25;
    float deltaX = xLast - xDistance;
    deltaX = abs(deltaX * speedConst);
    float deltaY = yLast - yDistance;
    deltaY = abs(deltaY * speedConst);
    float speedConstX = speedConst * abs(xDistance);
    float speedConstY = speedConst * abs(yDistance);


    if(xDistance > 0)
        xDistance = speedConstX  - min(deltaX,speedConstX);
    else
        xDistance = -1 * speedConstX + min(deltaX,speedConstX);
    if(yDistance > 0)
        yDistance = speedConstY - min(deltaY,speedConstY);
    else
        yDistance = -1 * speedConstY + min(deltaY,speedConstY);

    wheels.updateCommand(speedy + yDistance,speedx + xDistance,adjustedAngleRad);
    xLast = xDistance;
    yLast = yLast;
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

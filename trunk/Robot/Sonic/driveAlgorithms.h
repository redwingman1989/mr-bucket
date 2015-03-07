#pragma once

#include "Globals.h"

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

float getSpeedHelper(float offset,float lineCenter){
    float speedConst = 10;
    float speedConstCenter = 5;
    float centerThreshhold = .7;
    float speed = - 1 *(offset - lineCenter );

    if(speed < 0){
        speedConst *= -1;
    }

   if(abs(speed) > centerThreshhold){
        speed = speedConst;
   }
   else{
        speed *= speedConstCenter;
   }
   return speed;
}

void FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum){
    float turnConstant = 10;
    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    if(linePair.valid){
        float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
        adjustedAngleRad = convertRadToPercent(adjustedAngleRad,turnConstant);

        float speed = 0;
        if(abs(speedx) < abs(speedy)){
            speed =  getSpeedHelper(linePair.offset.x ,pairCenters[linePairEnum].x);
            wheels.updateCommand(speedy ,speed ,adjustedAngleRad);
        } else{
            speed =  getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y );
            wheels.updateCommand(speed , speedx ,adjustedAngleRad);
        }
    }else{
        wheels.updateCommand(0,0,0);
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

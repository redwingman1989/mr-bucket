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

float speedBuild(float *integral,float speed){
        float returnfloat = 0;
        if(abs(speed) < 1){
            *integral += speed;
        }
        if(abs(*integral) > 1){
            returnfloat =  *integral;
            *integral = 0;
        }else{
            returnfloat = speed;
        }
    return returnfloat;
}

bool FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum){
    float turnConstant = 8;
    static float totalAngle = 0;
    static float totalOff = 0;
    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    if(linePair.valid){
        float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
        adjustedAngleRad = convertRadToPercent(adjustedAngleRad,turnConstant);
        adjustedAngleRad = speedBuild(&totalAngle,adjustedAngleRad);

        float speed = 0;
        if(linePairEnum != LSP_BACK){
            speed = getSpeedHelper(linePair.offset.x ,pairCenters[linePairEnum].x);
            speed = speedBuild(&totalOff,speed);
            wheels.updateCommand(speedy ,speed ,adjustedAngleRad);
        } else{
            speed = getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y );
            speed = speedBuild(&totalOff,speed);
            wheels.updateCommand(speed , speedx ,adjustedAngleRad);
        }
    }else{
        wheels.updateCommand(0,0,0);
    }
    return false;
}

bool lineUpOneLine(lineSensorPairs linePairEnum){
    static float totalAngle = 0;
    static float totalOff = 0;
    float turnConstant = 8;
    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    if(linePair.valid){
        float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
        adjustedAngleRad = convertRadToPercent(adjustedAngleRad,turnConstant);

        if(abs(adjustedAngleRad) < 1){
            totalAngle += adjustedAngleRad;
        }
        if(abs(totalAngle) > 1){
            wheels.updateCommand(0 , 0 ,totalAngle);
            totalAngle = 0;
        }else{
            wheels.updateCommand(0 , 0 ,adjustedAngleRad);
        }
        /////
        float speed = 0;
        if(linePairEnum != LSP_BACK ){
            speed =  getSpeedHelper(linePair.offset.x ,pairCenters[linePairEnum].x);
            if(abs(speed) < 1){
            totalOff += adjustedAngleRad;
            }
            if(abs(totalOff) > 1){
                wheels.updateCommand(0 ,totalOff ,adjustedAngleRad);
                totalOff = 0;
                return true;
            }
            else
               wheels.updateCommand(0 ,speed ,adjustedAngleRad);
        }
         else{
            speed =  getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y );
            if(abs(speed) < 1){
                totalOff += adjustedAngleRad;
            }
            if(abs(totalOff) > 1){
                wheels.updateCommand(totalOff ,0 ,adjustedAngleRad);
                totalOff = 0;
                return true;
            }
            else
                wheels.updateCommand(speed , 0 ,adjustedAngleRad);
        }

    }else{
        wheels.updateCommand(0,0,0);
        }
        return false;
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

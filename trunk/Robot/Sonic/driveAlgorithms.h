#pragma once

#include "Globals.h"

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

float getSpeedHelper(float offset,float lineCenter){
    float speedConst = 2.5;
    float speedConstCenter = 5;
    float centerThreshhold = .5;
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
        return false;
    }
    return true;
}

//Returns true when on the line and centered
//Returns false when off the line or not centered
bool FollowLineSingle(float speedDirection, bool fwd, lineSensorLocations location){
    static float totalOff = 0;
    lineDriveCommand_t linePair = lineManager.getSingleCommand(location);

    float center;
    float offset;

    if (fwd) {
      offset = linePair.offset.x;
      center = sensorCenters[location].x;
    }
    else {
      offset = linePair.offset.y;
      center = sensorCenters[location].y;
    }

    if(linePair.valid){
        float speed = 0;
        speed = getSpeedHelper(offset ,center);
        speed = speedBuild(&totalOff,speed);
        if (fwd) wheels.updateCommand(speedDirection, speed, 0);
        else wheels.updateCommand(speed ,speedDirection ,0);
    }
    else{
        wheels.updateCommand(0,0,0);
        return false;
    }
    return (abs(offset - center) < 0.2);

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
            return true;
            }
            if(abs(totalOff) > 1){
                wheels.updateCommand(0 ,totalOff ,adjustedAngleRad);
                totalOff = 0;
            }
            else
               wheels.updateCommand(0 ,speed ,adjustedAngleRad);
        }
         else{
            speed =  getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y );
            if(abs(speed) < 1){
                totalOff += adjustedAngleRad;
                return true;
            }
            if(abs(totalOff) > 1){
                wheels.updateCommand(totalOff ,0 ,adjustedAngleRad);
                totalOff = 0;
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
  float returnVal;
  static float integralMotion=0;

  delta = desiredHeading - heading;
  if (delta > 180)
    delta -= 360;
  if (delta < -180)
    delta += 360;

  returnVal = (0.6 * delta);
  returnVal = speedBuild(&integralMotion,returnVal );

  return returnVal;
}

float getDeltaHeading(float desiredHeading){
    float delta;
    float heading = mag.getFiltHead();
    delta = desiredHeading - heading;
      if (delta > 180)
        delta -= 360;
      else if (delta < -180)
        delta += 360;
    return delta;
}

//Gets us to a heading going a direction unless +- 10 thresholdGap
float getToHeadingDirection(float desiredHeading,bool clockwise) {
  float delta;
  const float thresholdGap = 10;
  const float forceDirectionGap = 50;
  const float rotationSpeed = 4;
  const float smallRotationDivider = .3;
  static float integralMotion=0;

  delta = getDeltaHeading(desiredHeading);
  if( abs(delta) < forceDirectionGap){
    if(abs(delta) < thresholdGap){
        return speedBuild(&integralMotion, smallRotationDivider * delta );
    } else {
        if(delta < 0){
            return -rotationSpeed;
        } else{
            return rotationSpeed;
        }
    }
  }

  if( clockwise){
    return rotationSpeed;
  } else {
    return -rotationSpeed;
  }
}

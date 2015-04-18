#include "driveAlgorithms.h"

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

float getSpeedHelper(float offset,float lineCenter){
    float speedConst = 2.5;
    float speedConstCenter = 2.5;
    float centerThreshhold = 1;
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

float getSpeedHelper(float offset,float lineCenter,float speedConst){
    float speedConstCenter = 2.5;
    float centerThreshhold = 1;
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

float speedBuild(float *integral,float speed, float maxSpeed){
        float returnfloat = 0;
        if(abs(speed) < maxSpeed){
            *integral += speed;
        }
        if(abs(*integral) > maxSpeed){
            returnfloat =  *integral;
            *integral = 0;
        }else{
            returnfloat = speed;
        }
    return returnfloat;
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

bool FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum, bool firstTime){
    float turnConstant = 8;
    static float totalAngle = 0;
    static float totalOff = 0;
    if(firstTime){
        totalAngle = 0;
        totalOff = 0;
    }

    lineDriveCommand_t linePair = lineManager.getLineDriveCommand(linePairEnum);
    if(linePair.valid){
        float adjustedAngleRad = linePair.angle - pairAngleOffset[linePairEnum];
        adjustedAngleRad = convertRadToPercent(adjustedAngleRad,turnConstant);
        adjustedAngleRad = speedBuild(&totalAngle,adjustedAngleRad);

        float speed = 0;
        if(linePairEnum != LSP_BACK){
            speed = getSpeedHelper(linePair.offset.x ,pairCenters[linePairEnum].x,5);
            speed = speedBuild(&totalOff,speed);
            wheels.updateCommand(speedy ,speed ,adjustedAngleRad);
        } else{
            speed = getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y, 5);
            speed = speedBuild(&totalOff,speed);
            wheels.updateCommand(speed , speedx ,adjustedAngleRad);
        }
    }else{
        wheels.updateCommand(0,0,0);
        return false;
    }
    return true;
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
            speed = getSpeedHelper(linePair.offset.x ,pairCenters[linePairEnum].x,5);
            speed = speedBuild(&totalOff,speed);
            wheels.updateCommand(speedy ,speed ,adjustedAngleRad);
        } else{
            speed = getSpeedHelper(linePair.offset.y , pairCenters[linePairEnum].y, 5);
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
bool FollowLineSingle(float speedDirection, bool fwd, lineSensorLocations location, bool firstTime){
    static float totalOff = 0;
    static float AngleOff = 0;
    lineDriveCommand_t linePair = lineManager.getSingleCommand(location);

    float center;
    float offset;

    if(firstTime) {
      totalOff = 0;
      AngleOff = 0;
    }

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
        float driveAngle = 0;
        speed = getSpeedHelper(offset ,center);
        speed = speedBuild(&totalOff,speed,2);
        driveAngle = speedBuild(&AngleOff,speed * .02 * speedDirection);
        if (fwd) wheels.updateCommand(speedDirection, speed, driveAngle);
        else wheels.updateCommand(speed ,speedDirection ,driveAngle);
    }
    else{
        wheels.updateCommand(0,0,0);
        return false;
    }
    return (abs(offset - center) < 0.1);

}

//Returns true when on the line and centered
//Returns false when off the line or not centered
bool FollowLineSingle(float speedDirection, bool fwd, lineSensorLocations location, bool firstTime,bool turning){
    static float totalOff = 0;
    static float AngleOff = 0;
    lineDriveCommand_t linePair = lineManager.getSingleCommand(location);

    float center;
    float offset;

    if(firstTime) {
      totalOff = 0;
      AngleOff = 0;
    }

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
        float driveAngle = 0;
        speed = getSpeedHelper(offset ,center);
        speed = speedBuild(&totalOff,speed,2);
        if(turning)
            driveAngle = speedBuild(&AngleOff,speed * .02 * speedDirection);
        else
            driveAngle = 0;
        if (fwd) wheels.updateCommand(speedDirection, speed, driveAngle);
        else wheels.updateCommand(speed ,speedDirection ,driveAngle);
    }
    else{
        wheels.updateCommand(0,0,0);
        return false;
    }
    return (abs(offset - center) < 0.1);

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

  returnVal = (0.2 * delta);
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
float getToHeadingDirection(float desiredHeading,bool clockwise,float speed) {
  float delta;
  const float thresholdGap = 30;
  const float forceDirectionGap = 60;
  float rotationSpeed = speed;
  const float smallRotationDivider = .066;
  static float integralMotion=0;

  delta = getDeltaHeading(desiredHeading);
  if( abs(delta) < forceDirectionGap){
    if(abs(delta) < thresholdGap){
        return speedBuild(&integralMotion, smallRotationDivider * delta,1);
    } else {
        if(delta < 0){
            return -rotationSpeed;
        } else{
            return rotationSpeed;
        }
    }
  }
rotationSpeed = (speed - 2) * (abs(delta) - thresholdGap) * .0055 + 2;
  if( clockwise){
    return rotationSpeed;
  } else {
    return -rotationSpeed;
  }
}

float getToHeadingDirection(float desiredHeading,bool clockwise) {
    return getToHeadingDirection( desiredHeading, clockwise,2);
}

float scaleDistanceToSpeedCmd(
  float distance,
  float maxDistance,
  float minDistance,
  float maxSpeed,
  float minSpeed)
{
  float speedVal;

  /* Calculate Speed to move */
  if (distance > maxDistance)
    speedVal = maxSpeed;
  else if (distance < minDistance)
    speedVal = minSpeed;
  /* Linearly Scale Speed */
  else {
    speedVal = ((distance - minDistance)*(maxSpeed - minSpeed)/
               (maxDistance - minDistance)) + minSpeed;
  }
  return speedVal;
}

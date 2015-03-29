#pragma once

#include "Globals.h"

float convertRadToPercent(float rad, float speed);

float getSpeedHelper(float offset,float lineCenter);

float speedBuild(float *integral,float speed, float maxSpeed);

float speedBuild(float *integral,float speed);

bool FollowLine(float speedx,float speedy, lineSensorPairs linePairEnum);

//Returns true when on the line and centered
//Returns false when off the line or not centered
bool FollowLineSingle(float speedDirection, bool fwd, lineSensorLocations location);


bool lineUpOneLine(lineSensorPairs linePairEnum);
float getToHeading(float desiredHeading);

float getDeltaHeading(float desiredHeading);

//Gets us to a heading going a direction unless +- 10 thresholdGap
float getToHeadingDirection(float desiredHeading,bool clockwise);

/* retruns a speed command limited and linearly scaled to the input parameters */
float
scaleDistanceToSpeedCmd(
  float distance,
  float maxDistance,
  float minDistance,
  float maxSpeed,
  float minSpeed
);

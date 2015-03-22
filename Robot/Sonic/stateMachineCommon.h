#pragma once

#include "LineSensorManager.h"

typedef struct {
  bool staticButtonsDetected;
  uint8_t staticButtShadow;
  uint8_t staticButtTimeout;
  uint32_t staticStateTimeout;
} loadRingsSharedStaticData_t;

bool findCenterLine(bool first, float forwardSpeed, float sidewaysSpeed, float rotSpeed);

/* Load Rings: Check Buttons */
void loadRingsButtonDetection(
  lineSensorLocations lineLocation,
  loadRingsSharedStaticData_t * staticData);

/* Load Rings: Check for Timeout */
bool loadRingsTimeOutCheck (
  loadRingsSharedStaticData_t * staticData);

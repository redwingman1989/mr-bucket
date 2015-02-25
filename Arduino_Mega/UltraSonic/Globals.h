#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>
#include "Pins.h"
#include "Magnetometer.h"
#include "Buttons.h"
#include "MotorController.h"
#include "System\CycleUnit.h"
#include "LineSensorManager.h"
#include "Heartbeat.h"
#include <math.h>

/* I2C Address Constants */
static const uint8_t magI2CAddr = (0x1E);

/* Rates in Period of Micro Seconds */
static const uint32_t rate50Hz = (20000); // 50Hz Period In micro Seconds
static const uint32_t rate250Hz = (4000); // 250Hz Period In micro Seconds
static const uint32_t rate2Hz = (500000); // 2Hz Period In micro Seconds

/* Other Global Constants */
static const uint32_t serialBaud = (115200); // Debug Serial Baud Rate

/* Global Cycle Units */
CycleUnit sense;
CycleUnit plan;
CycleUnit act;

/* Global Sub-components of Runable Modules (not sure why these are global??)*/
LineSensor linesensorCenterFront(centerFront);
LineSensor linesensorCenterBack(centerBack);
LineSensor linesensorRightFront(sideFront);
LineSensor linesensorRightBack(sideBack);
LineSensor * linesensors[4];

/* Global Runable Modules (Inputs) */
ButtonManager buttMan;
LineSensorManager lineManager(linesensors);
Magnetometer mag(magI2CAddr);

/* Global Runable Modules (Outputs) */
Heartbeat heart(pinHbLed);
MotorController wheels;

#endif // _GLOBALS_H



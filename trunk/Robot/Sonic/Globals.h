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
#include "ServoArmController.h"
#include "UltraSonicManager.h"
#include "UltraSonicISR.h"
#include "MainExecMachine.h"
#include "primaryStateMachine.h"
#include <math.h>

/* I2C Address Constants */
static const uint8_t magI2CAddr = (0x1E);

/* Rates in Period of Micro Seconds */
static const uint32_t rate2Hz   = (500000); // 2Hz Period In micro Seconds
static const uint32_t rate16Hz  = (62500); // 16Hz Period in microseconds
static const uint32_t rate50Hz  = (20000); // 50Hz Period In micro Seconds
static const uint32_t rate100Hz = (10000); // 50Hz Period In micro Seconds
static const uint32_t rate250Hz = (4000); // 250Hz Period In micro Seconds

/* Competition Time */
static const uint32_t minutes_3 = (180000000); // Competition Timout of 3 minutes

/* Ring Load Time */
static const uint32_t ringLoadTime = (2 * 1000000); // Load Ring Time

/* Ring Load Time */
static const uint32_t armSwingTime = (4 * 1000000); // Arm Swing Ring Time

/* Other Global Constants */
static const uint32_t serialBaud = (115200); // Debug Serial Baud Rate

/* Global Cycle Units */
extern CycleUnit sense;
extern CycleUnit plan;
extern CycleUnit act;

/* Global Sub-components of Runable Modules (not sure why these are global??)*/
extern LineSensor linesensorCenterFront;
extern LineSensor linesensorCenterBack;
extern LineSensor linesensorRightFront;
extern LineSensor linesensorRightBack;
extern LineSensor * linesensors[4];

/* Global Runable Modules (Inputs) */
extern ButtonManager buttMan;
extern LineSensorManager lineManager;
extern Magnetometer mag;

/* Global Runable Modules (Outputs) */
extern Heartbeat heart;
extern MotorController wheels;
extern ServoArmController arm;
extern UltraSonicManager ultraSonicMgr;

extern PrimaryStateMachine masterChief;
//extern MainExecMachine mainExec;

#endif // _GLOBALS_H



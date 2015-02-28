#include "Globals.h"

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
ServoArmController arm;
UltraSonicManager ultraSonicMgr;

/* Global State Machine */

MainExecMachine mainExec;

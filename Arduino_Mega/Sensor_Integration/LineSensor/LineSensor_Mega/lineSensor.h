#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define LINE_DETECTION_TIME_IN_US (800)

unsigned long startCapChargeTime;
unsigned long frontLineSensorDischargeTimes[NUM_LINE_SENSOR_SENSORS];

unsigned char sensorDetectsWhite[NUM_LINE_SENSOR_SENSORS]; // Make this a bitfield

unsigned char portA;




#endif

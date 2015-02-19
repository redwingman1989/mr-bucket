#ifndef _LINESENSOR_INO_HEADER_H
#define _LINESENSOR_INO_HEADER_H

#include "UltraSonicSensor.h"

/* Global Variables */
extern UltraSonicSensor ultraSonicFront;
extern UltraSonicSensor ultraSonicLeft;
extern UltraSonicSensor ultraSonicRight;

/* Function Prototypes */
void cycle();
void heartbeat();
void PIT();


#endif // LINESENSOR_INO_HEADER_H

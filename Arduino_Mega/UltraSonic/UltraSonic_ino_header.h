#ifndef _LINESENSOR_INO_HEADER_H
#define _LINESENSOR_INO_HEADER_H

#include "UltraSonicSensor.h"

/* Global Variables */
extern UltraSonicManager ultraSonicMgr;

/* Function Prototypes */
void cycle();
void heartbeat();
void PIT();


#endif // LINESENSOR_INO_HEADER_H

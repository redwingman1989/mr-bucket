#ifndef _LINESENSORMGR_H
#define _LINESENSORMGR_H

#include "LineSensor.h"

/* Macros */

class LineSensorMgr {
  public:
    // Constructor
    LineSensorMgr(LineSensor * ptrLineSen1,
                  LineSensor * ptrLineSen2,
                  LineSensor * ptrLineSen3,
                  LineSensor * ptrLineSen4);
    // Destructor
    ~LineSensorMgr();

    // Get sensor Reading
    void takeAllSensorReadings();


  private:
      LineSensor * ptrSensor1;
      LineSensor * ptrSensor2;
      LineSensor * ptrSensor3;
      LineSensor * ptrSensor4;


};


#endif // _LINESENSORMGR_H

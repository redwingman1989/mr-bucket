#include <Arduino.h>
#include "LineSensor.h"
#include "LineSensorMgr.h"



LineSensorMgr::LineSensorMgr(LineSensor * ptr1,
                             LineSensor * ptr2,
                             LineSensor * ptr3,
                             LineSensor * ptr4):
                             ptrSensor1(ptr1),
                             ptrSensor2(ptr2),
                             ptrSensor3(ptr3),
                             ptrSensor4(ptr4)

{
;
}


LineSensorMgr::~LineSensorMgr()
{
    free(this);
}

void LineSensorMgr::takeAllSensorReadings()
{
    // Set the line high and make it an output
    this->ptrSensor1->beginCheck1();
    this->ptrSensor2->beginCheck1();
    this->ptrSensor3->beginCheck1();
    this->ptrSensor4->beginCheck1();
    // Charge the lines for 10 us
    delayMicroseconds(10);
    // Make the port and input and mark the start time
    this->ptrSensor1->beginCheck2();
    this->ptrSensor2->beginCheck2();
    this->ptrSensor3->beginCheck2();
    this->ptrSensor4->beginCheck2();
    //delay 300 us
    delayMicroseconds(300);

    this->ptrSensor1->takeReading();
    this->ptrSensor2->takeReading();
    this->ptrSensor3->takeReading();
    this->ptrSensor4->takeReading();
}

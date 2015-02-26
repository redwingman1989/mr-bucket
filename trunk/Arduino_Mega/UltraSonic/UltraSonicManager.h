#ifndef _ULTRASONICMANAGER_H
#define _ULTRASONICMANAGER_H


#include "UltraSonicSensor.h"
#include "System/RunableModule.h"


class UltraSonicManager : public RunableModule {
    public:
        UltraSonicManager();
        ~UltraSonicManager();

        /* Add an UltraSonic Sensor to the manager */
        void addSensor(uint8_t trigPin, uint8_t echoPin);

        /* Get the pointer to a requested sensor */
        UltraSonicSensor * getSensor(ultSensor_t sensor);

        /* Implement functions from the parent class */
        bool RunTick();
//        void DebugOutput(HardwareSerial * hwSerialPort);


    private:
      /* Number of managed sensors */
      uint8_t numManagedSensors;

      /* Array of UltraSonic Sensors */
      UltraSonicSensor arrManagedSensors[NUM_ULTRA_SENSORS];

      /* Which sensor should we exec next */
      ultSensor_t sensorToExec;


};

#endif // _ULTRASONICMANAGER_H

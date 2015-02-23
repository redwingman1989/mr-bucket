#ifndef _ULTRASONICMANAGER_H
#define _ULTRASONICMANAGER_H


#include "UltraSonicSensor.h"
#include "System/RunableModule.h"


class UltraSonicManager : public RunableModule {
    public:
        UltraSonicManager(UltraSonicSensor ** ptrUltraSonicObjArray);
        ~UltraSonicManager();

        /* Implement functions from the parent class */
        bool RunTick(uint16_t time, RobotState state);
        void DebugOutput(HardwareSerial * hwSerialPort);


    private:
        /* Array of pointers to UltraSonic Sensors */
        UltraSonicSensor ** ptrToUltraSonicObjArray;

        /* Which sensor should we exec next */
        uint8_t sensorToExec;


};

#endif // _ULTRASONICMANAGER_H

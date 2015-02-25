#pragma once
#include <Arduino.h>
enum RobotState{
  RS_Startup, //Waiting for push button to start, motors will be killed
  RS_LoadRings,
  RS_ScoreRings
};
class RunableModule
{
public:
    char nameStr[20];
    uint8_t debugLevel;

    RunableModule(void);
    ~RunableModule(void);
    //the main entry point for work each cycle
    virtual bool RunTick();
    //Shows us some debug
    virtual void DebugOutput(HardwareSerial *serialPort);

    bool setTimeOut(uint32_t);
    bool decTimer(uint32_t);

    int32_t getTimer() {return activeTimer;};

private:
  uint32_t timeOut;
  int32_t activeTimer;
};


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
    RunableModule(void);
    ~RunableModule(void);
    //the main entry point for work each cycle
    virtual bool RunTick(uint16_t time,RobotState state);
    //Shows us some debug
    virtual void DebugOutput(HardwareSerial *serialPort);
};


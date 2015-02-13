#pragma once
#include <Arduino.h>
enum RobotState{LoadRings,ScoreRings
};
class RunableModule
{
public:
    RunableModule(void);
    ~RunableModule(void);
    //the main entry point for work each cycle
    virtual bool RunTick(float time,RobotState state);
    //Shows us some debug
    virtual void DebugOutput(void);
};

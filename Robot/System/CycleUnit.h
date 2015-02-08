#pragma once
#include "RunableModule.h"
class CycleUnit
{
public:
    CycleUnit(void);
    ~CycleUnit(void);
    //list of modules to run
    RunableModule tasks[10];
    //main loop function
    void RunTasks(void);
};


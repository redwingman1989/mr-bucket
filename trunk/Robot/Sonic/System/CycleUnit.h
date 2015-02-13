#pragma once
#include "RunableModule.h"
#include <Arduino.h>
#include "LineSensor.h"
const int TaskListMax = 10;

class CycleUnit
{
public:
    CycleUnit(void);
    ~CycleUnit(void);
    //main loop function
    void RunTasks(float time,RobotState state);
    //False return if no task is added
    bool addTask(RunableModule * task);
protected:
    //list of modules to run
    RunableModule * tasks[TaskListMax];
    int taskSize;
};


#pragma once
#include "RunableModule.h"
#include <Arduino.h>

const int TaskListMax = 10;

class CycleUnit
{
public:
    CycleUnit(void);
    ~CycleUnit(void);
    //main loop function
    void RunTasks(uint16_t time,RobotState state);
    //False return if no task is added
    bool addTask(RunableModule *, uint32_t, uint8_t);
    bool addTask(RunableModule *, uint32_t, uint8_t, const char*);

    void setPrevMicro(uint32_t time) {prevMicro = time;};
protected:
    //list of modules to run
    RunableModule * tasks[TaskListMax];
    int taskSize;

private:
  uint32_t currentMicro;
  uint32_t prevMicro;
  uint32_t deltaMicro;
};


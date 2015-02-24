#include "CycleUnit.h"


CycleUnit::CycleUnit(void)
{
    taskSize = 0;
}


CycleUnit::~CycleUnit(void)
{
}

void CycleUnit::RunTasks(uint16_t time,RobotState state)
{
    currentMicro = micros();

    for(int i = 0; i < taskSize; i++){
        deltaMicro = currentMicro - prevMicro;
        if (tasks[i]->decTimer(deltaMicro)) {
          tasks[i]->RunTick();
          if (tasks[i]->debugEnable)
            tasks[i]->DebugOutput(&Serial);
            Serial.print("FOM: ");
            Serial.println(tasks[i]->getTimer());
        }
    }

    prevMicro = currentMicro;
}


bool CycleUnit::addTask(RunableModule * task, uint32_t schedTime, bool debug){
    if( taskSize < TaskListMax  ){
        tasks[taskSize] = task;
        tasks[taskSize]->setTimeOut(schedTime);
        tasks[taskSize]->debugEnable = debug;
        taskSize++;
        return true;
    }else{
        return false;
    }
}

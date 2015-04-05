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
    char dbHead[60];

    currentMicro = micros();

    for(int i = 0; i < taskSize; i++){
        deltaMicro = currentMicro - prevMicro;
        if (tasks[i]->decTimer(deltaMicro)) {
          tasks[i]->RunTick();
          if (tasks[i]->debugLevel) {
            sprintf(dbHead,"%s\nFOM: %ld\n",tasks[i]->nameStr,tasks[i]->getTimer());
            Serial2.print(dbHead);
            tasks[i]->DebugOutput(&Serial2);
          }
        }
    }

    prevMicro = currentMicro;
}


bool CycleUnit::addTask(RunableModule * task, uint32_t schedTime, uint8_t debug){
    if( taskSize < TaskListMax  ){
        tasks[taskSize] = task;
        tasks[taskSize]->setTimeOut(schedTime);
        tasks[taskSize]->debugLevel = debug;
        taskSize++;
        return true;
    }else{
        return false;
    }
}

bool
CycleUnit::addTask(
  RunableModule * task,
  uint32_t schedTime,
  uint8_t debug,
  const char * taskName)
{
    if( taskSize < TaskListMax  ){
        tasks[taskSize] = task;
        tasks[taskSize]->setTimeOut(schedTime);
        tasks[taskSize]->debugLevel = debug;
        strcpy(tasks[taskSize]->nameStr, taskName);
        taskSize++;
        return true;
    }else{
        return false;
    }
}

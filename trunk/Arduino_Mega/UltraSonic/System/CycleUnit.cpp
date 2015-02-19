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
    for(int i = 0; i < taskSize; i++){
        tasks[i]->RunTick(time,state);
        tasks[i]->DebugOutput(&Serial);
    }
}


bool CycleUnit::addTask(RunableModule * task){
    if( taskSize < TaskListMax  ){
        tasks[taskSize] = task;
        taskSize++;
        return true;
    }else{
        return false;
    }
}

#include "RunableModule.h"


RunableModule::RunableModule(void)
{
}


RunableModule::~RunableModule(void)
{
}

//main entry point
bool RunableModule::RunTick(float time,RobotState state){
    return false;
}

void RunableModule::DebugOutput(void){
    Serial.println("its working");
}

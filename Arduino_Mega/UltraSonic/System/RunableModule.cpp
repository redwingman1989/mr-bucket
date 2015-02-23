#include "RunableModule.h"


RunableModule::RunableModule(void)
{
}


RunableModule::~RunableModule(void)
{
}

//main entry point
bool RunableModule::RunTick(uint16_t time,RobotState state){
    return false;
}

void RunableModule::DebugOutput(HardwareSerial *serial){
    //Serial.println("its working");
}

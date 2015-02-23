#include "RunableModule.h"


RunableModule::RunableModule(void)
{
}


RunableModule::~RunableModule(void)
{
}

//main entry point
bool RunableModule::RunTick(){
    return false;
}

void RunableModule::DebugOutput(HardwareSerial *serial){
    Serial.println("its working");
}

bool RunableModule::setTimeOut(uint32_t time) {
  timeOut = time;
  activeTimer = (int32_t)timeOut;
  return true;
};

bool RunableModule::decTimer(uint32_t deltaTime){
  bool result = false;

  activeTimer -= deltaTime;
  if(activeTimer <= 0){
    activeTimer += (int32_t)timeOut;
    result = true;
  }

  return result;
};


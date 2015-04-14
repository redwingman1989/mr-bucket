#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H
#include "System/RunableModule.h"

class Heartbeat: public RunableModule {
  private:
    bool RunTick();
    void DebugOutput(HardwareSerial*);

    uint8_t pinAddr;
    bool ledState;

  public:
    Heartbeat(uint8_t addr);
    bool init();
};

#endif // _HEARTBEAT_H

#pragma once

#include "StateMachine.h"

typedef enum {
  Z_LOAD,
  Z_SCORE,
  Z_NUM_OF_ZONES
} Zone;

class SweepExecMachine: public StateMachine<SweepExecMachine> {
  public:
    SweepExecMachine(){};
    SweepExecMachine(Zone z);
    bool RunTick();

  private:

 };

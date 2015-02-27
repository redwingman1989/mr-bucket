#pragma once

#include "StateMachine.h"
#include "MainExecMachine.h"

class PrimaryStateMachine: public StateMachine <PrimaryStateMachine> {
public:
  PrimaryStateMachine();

private:
  uint32_t runTimeStart;
  void waitForStart(bool);
  void kickSomeAss(bool);
  void youWon(bool);

  /* Sub State Machines */
  MainExecMachine goToWork;
};

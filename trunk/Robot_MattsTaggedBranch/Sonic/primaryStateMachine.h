#pragma once

#include "StateMachine.h"
#include "MainExecMachine.h"
#include "DoublePointsStateMachine.h"

class PrimaryStateMachine: public StateMachine <PrimaryStateMachine> {
public:
  PrimaryStateMachine();

private:
  uint32_t runTimeStart;
  void waitForStart(bool);
  void kickSomeAss(bool);
  void doubleDown(bool);
  void youWon(bool);

  /* Sub State Machines */
  MainExecMachine goToWork;
  DpExecMachine bankBreaker;
};

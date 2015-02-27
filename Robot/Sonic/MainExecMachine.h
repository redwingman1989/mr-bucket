#pragma once

#include "StateMachine.h"

class MainExecMachine: public StateMachine <MainExecMachine> {
  public:
    MainExecMachine();

  private:
    uint32_t timeOut;
    uint16_t currentHeading;
    uint16_t desiredHeading;

    /* States */
    void loadLandR(bool);
    void backUp(bool);
    void shiftForCenter(bool);
    void loadCenter(bool);
    void flipABitch(bool);
    void haulAssToLoad(bool);
    void haulAssToScore(bool);
    void score(bool);

    /* Sub-Machines */

};

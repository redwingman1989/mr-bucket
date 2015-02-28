#pragma once

#include "StateMachine.h"

typedef enum {
  MEST_LOAD_LR_RINGS,
  MEST_BACKUP_ONE,
  MEST_SHIFT_FOR_CENTER,
  MEST_LOAD_CENTER_RINGS,
  MEST_BACKUP_TWO,
  MEST_FLIP_ONE,
  MEST_FIND_CENTER_LINE_ONE,
  MEST_HAUL_TOSCORE,
  MEST_SCORE,
  MEST_BACKUP_THREE,
  MEST_FLIP_TWO,
  MEST_FIND_CENTER_LINE_TWO,
  MEST_HAUL_TOLOAD,

  MEST_NUM_STATES,
} StateNum;

class MainExecMachine: public StateMachine<MainExecMachine> {
  public:
    MainExecMachine();

    void DebugOutput(HardwareSerial *);

  private:
    StateNum stateNum;
    uint32_t timeOut;
    uint16_t currentHeading;
    uint16_t desiredHeading;

    /* States */
    void loadRings(bool);
    void backUp(bool);
    void shiftForCenter(bool);
    void flipABitch(bool);
    void findCenterLine(bool);
    void haulAss(bool);
    void score(bool);

    /* Sub-Machines */

};
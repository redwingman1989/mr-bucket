#pragma once

#include "StateMachine.h"

typedef enum {
  MEST_LOAD_LR_RINGS,
  MEST_BACKUP_ONE,
  MEST_SHIFT_FOR_CENTER,
  MEST_LOAD_CENTER_RINGS,
  MEST_BACKUP_TWO,
  MEST_RAISE_ARM,
  MEST_FLIP_ONE,
  MEST_FIND_CENTER_LINE_ONE,
  MEST_HAUL_TOSCORE,
  MEST_SCORE,
  MEST_BACKUP_THREE,
  MEST_LOWER_ARM,
  MEST_FLIP_TWO,
  MEST_FIND_CENTER_LINE_TWO,
  MEST_HAUL_TOLOAD,

  MEST_NUM_STATES,
} StateNum;

class MainExecMachine: public StateMachine<MainExecMachine> {
  public:
    MainExecMachine();

    void DebugOutput(HardwareSerial *);
    void setScoreHead(float heading) {scoreHeading = heading;};
    void setLoadHead(float heading) {loadHeading = heading;};

  private:
    StateNum stateNum;
    uint32_t timeOut;
    float currentHeading;
    float desiredHeading;
    float scoreHeading;
    float loadHeading;

    /* States */
    void loadLeftRightRings(bool);
    void backupFromLeftRightRings(bool);
    void shiftForCenterRings(bool);
    void loadCenterRings(bool);
    void backupFromCenterRings(bool);
    void raiseArm(bool);
    void flipToScore(bool);
    void findCenterLineToScore(bool);
    void haulToScore(bool);
    void scoreRings(bool);
    void backupFromScoring(bool);
    void lowerArm(bool);
    void flipToLoad(bool);
    void findCenterLineToLoad(bool);
    void haulToLoad(bool);

};

#pragma once

#include "StateMachine.h"

typedef enum {
  MEST_LOAD_LR_RINGS, //0
  MEST_BACKUP_ONE, //1
  MEST_SHIFT_FOR_CENTER, //2
  MEST_LOAD_CENTER_RINGS, //3
  MEST_BACKUP_TWO, //4
  MEST_RAISE_ARM, //5
  MEST_FLIP_ONE, //6
  MEST_FIND_CENTER_LINE_ONE, //7
  MEST_HAUL_TOSCORE, //8
  MEST_SCORE, //9
  MEST_BACKUP_THREE, //10
  MEST_LOWER_ARM, //11
  MEST_FLIP_TWO, //12
  MEST_FIND_CENTER_LINE_TWO, //13
  MEST_HAUL_TOLOAD, //14

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

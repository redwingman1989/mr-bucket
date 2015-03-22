#pragma once

#include "StateMachine.h"

typedef enum {
  MEST_LOAD_LR_RINGS,         //0
  MEST_PICKUP_LR,             //1
  MEST_BACKUP_ONE,            //2
  MEST_SHIFT_FOR_CENTER,      //3
  MEST_LOAD_CENTER_RINGS,     //4
  MEST_PICKUP_CENTER,         //5
  MEST_BACKUP_TWO,            //6
  MEST_RAISE_ARM,             //7
  MEST_FLIP_ONE,              //8
  MEST_FIND_CENTER_LINE_ONE,  //9
  MEST_HAUL_TOSCORE,          //10
  MEST_SCORE,                 //11
  MEST_UNLOAD_ALL,            //12
  MEST_BACKUP_THREE,          //13
  MEST_LOWER_ARM,             //14
  MEST_FLIP_TWO,              //15
  MEST_FIND_CENTER_LINE_TWO,  //16
  MEST_HAUL_TOLOAD,           //17

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
    void pickupLeftRightRings(bool);
    void backupFromLeftRightRings(bool);
    void shiftForCenterRings(bool);
    void loadCenterRings(bool);
    void pickupCenterRings(bool);
    void backupFromCenterRings(bool);
    void raiseArm(bool);
    void flipToScore(bool);
    void findCenterLineToScore(bool);
    void haulToScore(bool);
    void scoreRings(bool);
    void unloadAllRings(bool);
    void backupFromScoring(bool);
    void lowerArm(bool);
    void flipToLoad(bool);
    void findCenterLineToLoad(bool);
    void haulToLoad(bool);

};

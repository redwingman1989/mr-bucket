#pragma once

#include "StateMachine.h"

typedef enum {
  Z_LOAD,
  Z_SCORE,

  Z_NUM_OF_ZONES
} Zone;

typedef enum {
  LZ_BACK_UP_INITIAL,
  LZ_SHIFT_LEFT_TO_WALL,
  LZ_DRIVE_FORWARD,
  LZ_SHIFT_RIGHT_TO_WALL,
  LZ_BACK_OFF_LEFT,
  LZ_BACK_UP_TO_ALIGN,
  LZ_REALIGN,

  LZ_NUM_OF_LZ_STATES
} LzState;

typedef enum {
  SZ_BACK_UP_INITIAL,
  SZ_SHIFT_RIGHT_TO_WALL,
  SZ_DRIVE_FORWARD,
  SZ_SHIFT_LEFT_TO_WALL,
  SZ_BACK_OFF_RIGHT,
  SZ_BACK_UP_TO_ALIGN,
  SZ_REALIGN,

  SZ_NUM_OF_SZ_STATES
} SzState;

class SweepExecMachine: public StateMachine<SweepExecMachine> {
  public:
    SweepExecMachine(){};
    SweepExecMachine(Zone z);
    bool RunTick();

  private:
    bool doneSweeping;

    uint8_t lzState;
    uint8_t szState;

    /* Sweep Load Zone Functions */
    void lz_backUpInitial(bool);
    void lz_shiftLeftToWall(bool);
    void lz_driveForward(bool);
    void lz_shiftRightToWall(bool);
    void lz_backOffLeft(bool);
    void lz_backUpToAlign(bool);
    void lz_reAlign(bool);

    /* Sweep Score Zone Functions */
    void sz_backUpInitial(bool);
    void sz_shiftLeftToWall(bool);
    void sz_driveForward(bool);
    void sz_shiftRightToWall(bool);
    void sz_backOffLeft(bool);
    void sz_backUpToAlign(bool);
    void sz_reAlign(bool);

 };



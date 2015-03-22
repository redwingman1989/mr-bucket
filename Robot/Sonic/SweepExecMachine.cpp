#include "SweepExecMachine.h"

SweepExecMachine::SweepExecMachine(Zone z) {
  /* Initalize State Arrays */
  const state lzStateArray[] = {
    (state) &SweepExecMachine::lz_backUpInitial,
    (state) &SweepExecMachine::lz_shiftLeftToWall,
    (state) &SweepExecMachine::lz_driveForward,
    (state) &SweepExecMachine::lz_shiftRightToWall,
    (state) &SweepExecMachine::lz_backOffLeft,
    (state) &SweepExecMachine::lz_backUpToAlign,
    (state) &SweepExecMachine::lz_reAlign
  };

  const state szStateArray[] = {
    (state) &SweepExecMachine::sz_backUpInitial,
    (state) &SweepExecMachine::sz_shiftRightToWall,
    (state) &SweepExecMachine::sz_driveForward,
    (state) &SweepExecMachine::sz_shiftLeftToWall,
    (state) &SweepExecMachine::sz_backOffRight,
    (state) &SweepExecMachine::sz_backUpToAlign,
    (state) &SweepExecMachine::sz_reAlign,
  };

  switch (z) {
    case Z_LOAD:
      lzState = LZ_BACK_UP_INITIAL;
      currentState = lzStateArray[lzState];
      break;
    case Z_SCORE:
    default:
      szState = SZ_BACK_UP_INITIAL;
      currentState = szStateArray[szState];
      break;
  }
}

bool SweepExecMachine::RunTick() {

}

/*--- Loading Zone Function Definitions ---*/
void SweepExecMachine::lz_backUpInitial(bool first){

}

void SweepExecMachine::lz_shiftLeftToWall(bool first){

}

void SweepExecMachine::lz_driveForward(bool first){

}

void SweepExecMachine::lz_shiftRightToWall(bool first){

}

void SweepExecMachine::lz_backOffLeft(bool first){

}

void SweepExecMachine::lz_backUpToAlign(bool first){

}

void SweepExecMachine::lz_reAlign(bool first){

}

/*--- Scoring Zone Function Definitions ---*/
void SweepExecMachine::sz_backUpInitial(bool first){

}

void SweepExecMachine::sz_shiftRightToWall(bool first){

}

void SweepExecMachine::sz_driveForward(bool first){

}

void SweepExecMachine::sz_shiftLeftToWall(bool first){

}

void SweepExecMachine::sz_backOffRight(bool first){

}

void SweepExecMachine::sz_backUpToAlign(bool first){

}

void SweepExecMachine::sz_reAlign(bool first){

}

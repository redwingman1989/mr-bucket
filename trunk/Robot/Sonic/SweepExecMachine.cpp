
#include "SweepExecMachine.h"

SweepExecMachine::SweepExecMachine(Zone z) {
  switch (z) {
    case Z_LOAD:
      break;
    case Z_SCORE:
    default:
      break;
  }
}

bool SweepExecMachine::lz_RunTick() {
  return true;
}

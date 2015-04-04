#pragma once

#include "StateMachine.h"

class DpExecMachine: public StateMachine<DpExecMachine> {
  public:
    DpExecMachine();
    bool RunTick();
    float scoreHeading;
    float loadHeading;
    float centerPoleHeading;

  private:
    bool doneDoublingPoints;

    /* */
    void backUpInitial(bool);
    void rotateToHeading(bool);
    void backUpToWall(bool);
    void shiftForCenterPost(bool);
    void deployTheSecretWeapon(bool);
    void retractTheDpServo(bool);
    void avoidHovering(bool);

    void calcCenterPoleHeading(void) {
      float temp = scoreHeading - loadHeading;
      if (temp < 0) temp += 360;

      temp /= 2.0;

      centerPoleHeading = loadHeading + temp;

      if (centerPoleHeading > 360) centerPoleHeading-=360;
    }
 };

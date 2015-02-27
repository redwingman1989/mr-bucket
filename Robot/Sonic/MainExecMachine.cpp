#include "MainExecMachine.h"
#include "Globals.h"

MainExecMachine::MainExecMachine() {
  currentState = (state) &MainExecMachine::loadLandR;
}


void MainExecMachine::loadLandR(bool firstTime) {
  static bool buttonsDetected = 0;

  /* Move Forward Till we hit Buttons */

  /* Buttons Detected */
  if (buttMan.getButtons() & 0x03 == 0x03) {
    timeOut = micros();
    buttonsDetected = true;
  }
  /* Command left and right servos to lift rings */

  /* Wait for top Servos to lift Rings on LEft and Right Pegs */

  /* We waited long enough, change state to back up */
  if (micros() - timeOut > ringLoadTime) {
    currentState = (state) &MainExecMachine::backUp;
    buttonsDetected = false;
  }
}

void MainExecMachine::backUp(bool firstTime) {
  /* if we just transistioned to this state*/

  /* If previous state was loadLandR */
  /* Back up until the two back sensors are on the line */
  /* Double Check ultrasonic is within tolerable range */
  /* Tansistion to ShiftForCenter */

  /* If Previous state was loadCenter */
  /* Backup based on ultrasonic up to 12 inches +/- */
  /* make sure back line sensors do not detect any lines */
  /* Transistion to flipABitch */
}

void MainExecMachine::shiftForCenter(bool firstTime) {
  /* shift to the right until the front center detects line on center front for the second time */

  /* once the front Center, and two back line censors are centered up, transition to loadCenter() */
}

void MainExecMachine::loadCenter(bool firstTime) {
    static bool buttonsDetected = 0;

  /* Move Forward Till we hit Buttons */

  /* Buttons Detected */
  if (buttMan.getButtons() & 0x03 == 0x03) {
    timeOut = micros();
    buttonsDetected = true;
    /* Command Center servos to lift rings */
  }

  // Save off Heading from magnetometer for later 180 flip

  /* Wait for top Servos to lift Rings on Center Peg */

  /* We waited long enough, change state to back up */
  if (micros() - timeOut > ringLoadTime) {
    currentState = (state) &MainExecMachine::backUp;
    buttonsDetected = false;
  }
}

void MainExecMachine::flipABitch(bool firstTime) {
  //

  // rotate till current heading is within tollerance of desired heading

  // once the desired heading is reached
  //   transition to haulAss
}

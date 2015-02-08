#include <Arduino.h>
#include <Servo.h>


#ifndef ServoArmController_h
#define ServoArmController_h

typedef struct {
  int down;
  int up;
} swingArmStops_t;

typedef enum {
  PU_LEFT,
  PU_CENTER,
  PU_RIGHT,
  NUM_OF_PICKUP_SERVOS
} pickupServoSelection_t;

typedef enum {
  SA_DOWN,
  SA_UP,
  NUM_OF_SWINGARM_STATES
} swingArmStates_t;

typedef enum {
  PS_LETGO,
  PS_GRAB,
  NUM_OF_PICKUP_STATES
} pickupStates_t;

class ServoArmController
{
public:
  ServoArmController();
  void commandSwingArm(swingArmStates_t state);
  void commandPickupServo(pickupServoSelection_t selection, pickupStates_t state);
  void exec(void);
  void init(void);

private:
  Servo swingArm;
  Servo pickupLeft;
  Servo pickupCenter;
  Servo pickupRight;

  int swingArmConstants[NUM_OF_SWINGARM_STATES];
  int pickupConstants[NUM_OF_PICKUP_SERVOS][NUM_OF_SWINGARM_STATES][NUM_OF_PICKUP_STATES];

  swingArmStates_t currentSwingArmState;
  pickupStates_t currentPickupState[NUM_OF_PICKUP_SERVOS];

  int swingArmCommand;
  int pickupLeftCommand;
  int pickupCenterCommand;
  int pickupRightCommand;
};

#endif // ServoArmController_h

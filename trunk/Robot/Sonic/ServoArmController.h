#ifndef ServoArmController_h
#define ServoArmController_h

#include <Arduino.h>
#include <Servo.h>
#include "System/RunableModule.h"

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
  DP_RETRACT,
  DP_DROP,
  NUM_OF_DOUBLE_POINT_STATES
} doublePointStates_t;

typedef enum {
  PS_LETGO,
  PS_GRAB,
  NUM_OF_PICKUP_STATES
} pickupStates_t;

class ServoArmController : public RunableModule
{
public:
  ServoArmController();
  void commandSwingArm(swingArmStates_t state);
  void commandPickupServo(pickupServoSelection_t selection, pickupStates_t state);
  void commandDoublePointServo(doublePointStates_t state);
  bool RunTick(void);
  void DebugOutput(HardwareSerial *serialPort);
  void init(void);
  void setPickupArmLimit(uint8_t sec);
  void setSwingArmLimit(uint8_t sec);

private:
  Servo swingArm;
  Servo pickupLeft;
  Servo pickupCenter;
  Servo pickupRight;
  Servo doublePoint;

  int swingArmConstants[NUM_OF_SWINGARM_STATES];
  int pickupConstants[NUM_OF_PICKUP_SERVOS][NUM_OF_SWINGARM_STATES][NUM_OF_PICKUP_STATES];
  int doublePointConstants[NUM_OF_DOUBLE_POINT_STATES];

  swingArmStates_t currentSwingArmState;
  pickupStates_t currentPickupState[NUM_OF_PICKUP_SERVOS];
  doublePointStates_t currentDoublePointState;

  int swingArmCommand;
  int pickupLeftCommand;
  int pickupCenterCommand;
  int pickupRightCommand;
  int doublePointCommand;

  int swingArmLimit;
  int pickupLimit;
  int doublePointLimit;
};

#endif // ServoArmController_h

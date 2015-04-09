#include <Arduino.h>
#include "ServoArmController.h"
#include "System\CycleUnit.h"

/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
CycleUnit armTest;
ServoArmController arm;

typedef enum {
  SL_swingArm,
  SL_pickupLeft,
  SL_pickupCenter,
  SL_pickupRight,
  SL_doublePoint
} servoList;

void printServoTable(void);
void printCurrentServo(servoList);

void setup()
{
	Serial.begin(115200);

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	pinMode(13, OUTPUT);

	arm.init();
	armTest.addTask(&arm, 20000, 0);
}

void loop()
{
  int temp;
  static servoList selectedServo = SL_swingArm;
  static int increment = 5;
  static int shadowIncrement = 5;
  static int* currentCommand;
  static int* commandShadow;
  static int commandValueShadow;

  armTest.RunTasks(millis(), RS_Startup);

  //Update servo command pointer
  if (selectedServo == SL_swingArm) currentCommand = &(arm.swingArmConstants[arm.currentSwingArmState]);
  else if (selectedServo == SL_pickupLeft) currentCommand = &(arm.pickupConstants[PU_LEFT][arm.currentSwingArmState][arm.currentPickupState[PU_LEFT]]);
  else if (selectedServo == SL_pickupCenter) currentCommand = &(arm.pickupConstants[PU_CENTER][arm.currentSwingArmState][arm.currentPickupState[PU_CENTER]]);
  else if (selectedServo == SL_pickupRight) currentCommand = &(arm.pickupConstants[PU_RIGHT][arm.currentSwingArmState][arm.currentPickupState[PU_RIGHT]]);
  else if (selectedServo == SL_doublePoint) currentCommand = &(arm.doublePointConstants[arm.currentDoublePointState]);

  if (currentCommand != commandShadow) {
    printCurrentServo(selectedServo);
    Serial.println("selected");
  }

  else if (*currentCommand != commandValueShadow) {
    printCurrentServo(selectedServo);
    Serial.print("= ");
    Serial.println(*currentCommand);
  }

  commandShadow = currentCommand;
  commandValueShadow = *currentCommand;

  //Grab serial input and parse commands
  while (Serial.available() > 0)
  {
    temp = Serial.read();

    //Print current table
    if (temp == 'p') printServoTable();

    //Change selected servo
    else if (temp == 'c') {
      selectedServo = (servoList)((int)selectedServo + 1);
      if (selectedServo > SL_doublePoint) selectedServo = SL_swingArm;
    }

    //Increment command for selected servo
    else if (temp == '+') {
      if (currentCommand) *currentCommand+=increment;
    }

    //Decrement command for selected servo
    else if (temp == '-') {
      if (currentCommand) *currentCommand-=increment;
    }

    //Command arm
    else if (temp == '1') arm.commandSwingArm(SA_DOWN);
    else if (temp == '2') arm.commandSwingArm(SA_UP);
    else if (temp == '3') arm.commandPickupServo(PU_LEFT, PS_LETGO);
    else if (temp == '4') arm.commandPickupServo(PU_LEFT, PS_GRAB);
    else if (temp == '5') arm.commandPickupServo(PU_CENTER, PS_LETGO);
    else if (temp == '6') arm.commandPickupServo(PU_CENTER, PS_GRAB);
    else if (temp == '7') arm.commandPickupServo(PU_RIGHT, PS_LETGO);
    else if (temp == '8') arm.commandPickupServo(PU_RIGHT, PS_GRAB);
    else if (temp == '9') arm.commandDoublePointServo(DP_RETRACT);
    else if (temp == '0') arm.commandDoublePointServo(DP_DROP);

    //End of checking temp
    else;



    //Limit commands
    if (*currentCommand > 3000) *currentCommand = 3000;
    if (*currentCommand < 100) *currentCommand = 100;

    //Print if increment changes
    if (increment != shadowIncrement) {
      Serial.print("Increment = ");
      Serial.println(increment);
      shadowIncrement = increment;
    }

  }
}

void printServoTable(void) {
  Serial.println();

  Serial.println("Swing Arm:");
  Serial.print("Down = ");
  Serial.println(arm.swingArmConstants[SA_DOWN]);
  Serial.print("Up = ");
  Serial.println(arm.swingArmConstants[SA_UP]);
  Serial.println();
  delay(100);

  Serial.println("Pickup Left");
  Serial.print("Down Let Go = ");
  Serial.println(arm.pickupConstants[PU_LEFT][SA_DOWN][PS_LETGO]);
  Serial.print("Down Grab= ");
  Serial.println(arm.pickupConstants[PU_LEFT][SA_DOWN][PS_GRAB]);
  Serial.print("Up Let Go = ");
  Serial.println(arm.pickupConstants[PU_LEFT][SA_UP][PS_LETGO]);
  Serial.print("Up Grab = ");
  Serial.println(arm.pickupConstants[PU_LEFT][SA_UP][PS_GRAB]);
  Serial.println();
  delay(100);

  Serial.println("Pickup Center");
  Serial.print("Down Let Go = ");
  Serial.println(arm.pickupConstants[PU_CENTER][SA_DOWN][PS_LETGO]);
  Serial.print("Down Grab= ");
  Serial.println(arm.pickupConstants[PU_CENTER][SA_DOWN][PS_GRAB]);
  Serial.print("Up Let Go = ");
  Serial.println(arm.pickupConstants[PU_CENTER][SA_UP][PS_LETGO]);
  Serial.print("Up Grab = ");
  Serial.println(arm.pickupConstants[PU_CENTER][SA_UP][PS_GRAB]);
  Serial.println();
  delay(100);

  Serial.println("Pickup Right");
  Serial.print("Down Let Go = ");
  Serial.println(arm.pickupConstants[PU_RIGHT][SA_DOWN][PS_LETGO]);
  Serial.print("Down Grab= ");
  Serial.println(arm.pickupConstants[PU_RIGHT][SA_DOWN][PS_GRAB]);
  Serial.print("Up Let Go = ");
  Serial.println(arm.pickupConstants[PU_RIGHT][SA_UP][PS_LETGO]);
  Serial.print("Up Grab = ");
  Serial.println(arm.pickupConstants[PU_RIGHT][SA_UP][PS_GRAB]);
  Serial.println();
  delay(100);

  Serial.println("Double Point");
  Serial.print("Retract = ");
  Serial.println(arm.doublePointConstants[DP_RETRACT]);
  Serial.print("Drop = ");
  Serial.println(arm.doublePointConstants[DP_DROP]);
  Serial.println();
  delay(100);
}

void printCurrentServo(servoList selectedServo) {
  //Print out selected servo
  if (selectedServo == SL_swingArm) Serial.print("Swing Arm ");
  else if (selectedServo == SL_pickupLeft) Serial.print("Pickup Left ");
  else if (selectedServo == SL_pickupCenter) Serial.print("Pickup Center ");
  else if (selectedServo == SL_pickupRight) Serial.print("Pickup Right ");
  else if (selectedServo == SL_doublePoint) Serial.print("Double Point ");

  if (selectedServo != SL_doublePoint) {
    if (arm.currentSwingArmState == SA_DOWN) Serial.print("Down ");
    else Serial.print("Up ");
  }
  else {
    if (arm.currentDoublePointState == DP_DROP) Serial.print("Drop ");
    else Serial.print("Retract ");
  }

  if (selectedServo == SL_pickupLeft) {
    if (arm.currentPickupState[PU_LEFT] == PS_LETGO) Serial.print("Let Go ");
    else Serial.print("Grab ");
  }

  if (selectedServo == SL_pickupCenter) {
    if (arm.currentPickupState[PU_CENTER] == PS_LETGO) Serial.print("Let Go ");
    else Serial.print("Grab ");
  }

  if (selectedServo == SL_pickupRight) {
    if (arm.currentPickupState[PU_RIGHT] == PS_LETGO) Serial.print("Let Go ");
    else Serial.print("Grab ");
  }
}

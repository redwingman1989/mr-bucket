#include <Arduino.h>
#include "StateMachine.h"
#include "System/CycleUnit.h"

CycleUnit doTheThing;

//Make a state machine - note that LEDStateMachine was created inside LEDWrapper
LEDWrapper wrapperSM;

void setup()
{
  Serial.begin(115200);
  Serial.println("START");

  //Add state machine as a task at 50HZ (period 20K us)
  doTheThing.addTask(&wrapperSM, 20000, true);
}

void loop()
{
  //Run state machine as a task
  doTheThing.RunTasks(0,RS_LoadRings);
}


#include <Arduino.h>
#include "StateMachine.h"

LEDWrapper wrapper;

void setup()
{
  delay(15);
  Serial.begin(9600);
  Serial.println("START");
}

void loop()
{
  delay(500);
  wrapper.RunTick();
}


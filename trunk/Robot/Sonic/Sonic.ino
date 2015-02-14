#include <Arduino.h>
#include "LineSensor.h"
#include "System\CycleUnit.h"
/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
CycleUnit sense;
LineSensor* centerFrontLineSensor = new LineSensor(centerFront);
LineSensor* sideFrontLineSensor = new LineSensor(sideFront);
LineSensor* centerBackLineSensor = new LineSensor(centerBack);
LineSensor* sideBackLineSensor = new LineSensor(sideBack);

void setup()
{
	Serial.begin(57600);

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	pinMode(13, OUTPUT);
}

void loop()
{
    static bool light = true;
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);
    //Plan
    //Act
    digitalWrite(13, light != light);   // set the LED on
}

#include <Arduino.h>
#include "LineSensor.h"
#include "Magnetometer.h"
#include "System\CycleUnit.h"
/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
CycleUnit sense;

Magnetometer* mag;


void setup()
{
	Serial.begin(57600);

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	mag = new Magnetometer(0x1E);
	pinMode(13, OUTPUT);

	sense.addTask(mag);
}

void loop()
{
    static bool light = true;
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);
    //Plan
    delay(20);
    //Act
    digitalWrite(13, light != light);   // set the LED on
}

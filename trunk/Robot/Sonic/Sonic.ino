#include <Arduino.h>
#include "LineSensor.h"
/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
LineSensor testFront(PORTA,DDRA);
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
    testFront.beginCheck();
    delayMicroseconds(300);
    testFront.getReading();
    Serial.println(testFront.sensorReadings.allReadings,BIN);
    digitalWrite(13, light != light);   // set the LED on
}

#include <Arduino.h>
#include "LineSensor.h"
#include "Magnetometer.h"
#include "MotorController.h"
#include "System\CycleUnit.h"
/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
CycleUnit sense;

Magnetometer mag(0x1E);
MotorController wheels;


void setup()
{
	Serial.begin(57600);

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	//mag = new Magnetometer(0x1E);
	mag.init();
	wheels.init();

	pinMode(13, OUTPUT);

	sense.addTask(&mag);
	sense.addTask(&wheels);
}

void loop()
{
    const uint16_t desiredAngle = 180;
    const uint8_t deadBand = 2;
    int16_t delta;
    int8_t rotSpeed;
    static bool light = true;
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);
    //Plan

    delta = desiredAngle - mag.getFiltHead();

    if (delta >= 0-deadBand){
        rotSpeed = 35;
    }
    else if (delta <= 0-deadBand) {
        rotSpeed = -35;
    }
    else{
        rotSpeed = 0;
    }

    wheels.updateCommand(0, 0, rotSpeed);

    delay(20);
    //Act
    digitalWrite(13, light != light);   // set the LED on
}

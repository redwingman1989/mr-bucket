#include <Arduino.h>
#include "Magnetometer.h"
#include "MotorController.h"
#include "System\CycleUnit.h"
#include "LineSensorManager.h"

CycleUnit sense;
CycleUnit plan;
CycleUnit act;

Magnetometer mag(0x1E);
//////////////////
/////Line Sensors
/////////////////
LineSensor linesensorCenterFront(centerFront);
LineSensor linesensorCenterBack(centerBack);
LineSensor linesensorRightFront(sideFront);
LineSensor linesensorRightBack(sideBack);
LineSensor * linesensors[4];
LineSensorManager lineManager(linesensors);
///////

MotorController wheels;


void setup()
{
	Serial.begin(57600);

	linesensors[LSL_CENTER_FRONT] = &linesensorCenterFront;
    linesensors[LSL_CENTER_BACK] =  &linesensorCenterBack;
    linesensors[LSL_RIGHT_FRONT] = &linesensorRightFront;
    linesensors[LSL_RIGHT_BACK] = &linesensorRightBack;

	// initialize the digital pin as an output.
	// Pin 13 has an LED connected on most Arduino boards:
	//mag = new Magnetometer(0x1E);
	mag.init();
	wheels.init();

	pinMode(13, OUTPUT);

	sense.addTask(&mag);
	sense.addTask(&lineManager);

	act.addTask(&wheels);
}

void loop()
{
    const uint16_t desiredAngle = 180;
    const uint8_t deadBand = 1;
    int16_t delta;
    const uint8_t maxSpd = 35;
    const uint8_t minSpd = 20;
    int8_t rotSpeed;
    static bool light = true;
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);

    delay(20);
    //Act
    digitalWrite(13, light != light);   // set the LED on
}

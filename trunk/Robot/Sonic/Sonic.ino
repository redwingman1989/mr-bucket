#include <Arduino.h>
#include "Magnetometer.h"
#include "Buttons.h"
#include "MotorController.h"
#include "System\CycleUnit.h"
#include "LineSensorManager.h"

CycleUnit sense;
CycleUnit plan;
CycleUnit act;

Magnetometer mag(0x1E);
ButtonManager buttMan;
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
  Serial.begin(115200);

  mag.init();

  buttMan.init();
  buttMan.addButton(LBUTT);
  buttMan.addButton(RBUTT);

  linesensors[LSL_CENTER_FRONT] = &linesensorCenterFront;
  linesensors[LSL_CENTER_BACK]  = &linesensorCenterBack;
  linesensors[LSL_RIGHT_FRONT]  = &linesensorRightFront;
  linesensors[LSL_RIGHT_BACK]   = &linesensorRightBack;

  wheels.init();

  sense.addTask(&mag);
  sense.addTask(&buttMan);
  sense.addTask(&lineManager);

  //act.addTask(&wheels);
}

void loop()
{
  static bool light = true;
  //Sense
  sense.RunTasks(millis(),RS_LoadRings);

  delay(20);
  //Act
}

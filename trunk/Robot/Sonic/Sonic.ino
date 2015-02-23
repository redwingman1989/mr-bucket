#include <Arduino.h>
#include "Magnetometer.h"
#include "Buttons.h"
#include "MotorController.h"
#include "System\CycleUnit.h"
#include "LineSensorManager.h"
#include <math.h>

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

	act.addTask(&wheels);
}

float convertRadToPercent(float rad){
    return rad / PI * 50.0 * 4.0;
}

void loop()
{
    static bool light = true;
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);
    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);
    if(rightPair.valid){
            Serial.println("rightpair is valid");
        if(! backPair.valid){
            float driveSpeed = 30;
            float adjustedAngleRad = rightPair.angle - PI / 2.0;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad);
            Serial.println(adjustedAngleRad , 4);
            float xDistance = (rightPair.offset.x -  sensorCenters[LSL_RIGHT_FRONT].x ) / sensorCenters[LSL_RIGHT_FRONT].x * 10;
            //float maxScale = xDistance + adjustedAngleRad;
            wheels.updateCommand(driveSpeed,xDistance,adjustedAngleRad);
        }
        else {
            wheels.updateCommand(0,0,0);
            delay(2000);
        }
    }
    else wheels.updateCommand(0,0,0);
    //act
    act.RunTasks(millis(),RS_LoadRings);


    delay(20);
    //Act
    digitalWrite(13, light = !light);   // set the LED on
}

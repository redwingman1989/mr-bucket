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
void followRightLine(void);
void reverseIt(void);
void uTurn(void);

enum states{
    FOLLOW_RIGHT_LINE,
    REVERSE_IT,
    U_TURN
};

int state = FOLLOW_RIGHT_LINE;
int prevState = 0;
int stateChange = true;

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

  sense.addTask(&mag);
  sense.addTask(&lineManager);
  sense.addTask(&buttMan);
}

float convertRadToPercent(float rad){
    return rad / PI * 50.0 * 4.0;
}

void loop()
{
    static bool light = true;

    //Sense
    sense.RunTasks(millis(),RS_LoadRings);

    //act
    act.RunTasks(millis(),RS_LoadRings);


    delay(20);
    //Act

    switch (state) {
    case FOLLOW_RIGHT_LINE:
        followRightLine();
        Serial.println("follow");
        break;
    case REVERSE_IT:
        reverseIt();
        Serial.println("reverse");
        break;
    case U_TURN:
        uTurn();
        Serial.println("uturn");
        break;
    }

    if (prevState != state) stateChange = true;
    else stateChange = false;
    prevState = state;
}

void followRightLine(void) {
    static bool buttFlag = false;
    static int buttTime = 0;

    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);

    if (buttMan.getButtons() == 0b11) {
        if (!buttFlag) buttTime = millis();
        buttFlag = true;
    }
    else buttFlag = false;

    if (buttFlag && (millis()-buttTime > 500)) {
       state = REVERSE_IT;
       buttFlag = false;
    }

    if(rightPair.valid){
            Serial.println("rightpair is valid");
        if(! backPair.valid){
            float driveSpeed = 30;
            float adjustedAngleRad = rightPair.angle - PI / 2.0;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad);
            Serial.println(adjustedAngleRad , 4);
            float xDistance = (rightPair.offset.x -  sensorCenters[LSL_RIGHT_FRONT].x ) / 1.5 * 30;
            //float maxScale = xDistance + adjustedAngleRad;
            wheels.updateCommand(driveSpeed,xDistance,adjustedAngleRad);
        }
        else {
            wheels.updateCommand(0,0,0);
            //delay(2000);
        }
    }
    else wheels.updateCommand(0,0,0);

}

void reverseIt(void) {
    static int stateChangeTime = 0;
    if (stateChange) stateChangeTime = millis();
    wheels.updateCommand(-50,0,0);
    if (millis() - stateChangeTime > 1000) state = U_TURN;
}

void uTurn(void) {
    static int16_t desiredHeading = 0;
    int16_t heading = mag.getFiltHead();
    int16_t delta;
    if (stateChange) {
        desiredHeading = heading + 180;
        if (desiredHeading > 360) desiredHeading -= 360;
    }

    delta = desiredHeading - heading;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    if (abs(delta) <= 3) {
        state = FOLLOW_RIGHT_LINE;
        wheels.updateCommand(0,0,0);
    }
    else wheels.updateCommand(0,0,0.5*delta);
}

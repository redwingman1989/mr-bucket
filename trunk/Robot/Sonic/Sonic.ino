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
uint16_t desiredHeading;

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

  act.addTask(&wheels, 20000, true);

  sense.addTask(&mag, 4000, false);
  sense.addTask(&lineManager, 4000, false);
  sense.addTask(&buttMan, 4000, true);

  sense.setPrevMicro(micros());
  act.setPrevMicro(micros());
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

    //Act

    switch (state) {
    case FOLLOW_RIGHT_LINE:
        followRightLine();
        //Serial.println("follow");
        break;
    case REVERSE_IT:
        reverseIt();
        //Serial.println("reverse");
        break;
    case U_TURN:
        uTurn();
        //Serial.println("uturn");
        break;
    }

    if (prevState != state) stateChange = true;
    else stateChange = false;
    prevState = state;


}

void followRightLine(void) {
    static bool buttFlag = false;
    static unsigned long buttTime = 0;

    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);

    if(rightPair.valid){
            //Serial.println("rightpair is valid");
        if(! backPair.valid){
            float driveSpeed = 30;
            float adjustedAngleRad = rightPair.angle - PI / 2.0;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad);
            //Serial.println(adjustedAngleRad , 4);
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


    if (buttMan.getButtons() == 0b11) {
        if (!buttFlag) buttTime = millis();
        wheels.updateCommand(0,0,0);
        buttFlag = true;
    }

    if (buttFlag && (millis()-buttTime > 2000)) {
       state = REVERSE_IT;
       buttFlag = false;
    }

}

void reverseIt(void) {
    static unsigned long stateChangeTime = 0;
    if (stateChange) {
        desiredHeading = mag.getFiltHead() + 180;
        if (desiredHeading > 360) desiredHeading -= 360;
        stateChangeTime = millis();
    }
    wheels.updateCommand(-50,0,0);
    if (millis() - stateChangeTime > 500) state = U_TURN;
}

void uTurn(void) {
    int16_t heading = mag.getRawHead();
    int16_t delta;


    delta = desiredHeading - heading;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;

    if (abs(delta) <= 3) {
        state = FOLLOW_RIGHT_LINE;
        wheels.updateCommand(0,0,0);
    }
    else {
        if (delta>0) wheels.updateCommand(0,0,(0.2*delta)+25);
        else wheels.updateCommand(0,0,-25 + (0.2*delta));
    }
}

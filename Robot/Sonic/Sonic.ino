#include "Globals.h"

void bullShitDemoCode(void);
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

float convertRadToPercent(float rad, float speed){
    return rad / PI * speed * 4.0;
}

void setup()
{
  Serial.begin(serialBaud);

  /*--- Initialize Runable Modules ---*/
  /* Inputs */
  buttMan.init();
  buttMan.addButton(pinLButton); // Button 0
  buttMan.addButton(pinRButton); // Button 1
  buttMan.addButton(pinGrnButton); // Button 2

  linesensors[LSL_CENTER_FRONT] = &linesensorCenterFront;
  linesensors[LSL_CENTER_BACK]  = &linesensorCenterBack;
  linesensors[LSL_RIGHT_FRONT]  = &linesensorRightFront;
  linesensors[LSL_RIGHT_BACK]   = &linesensorRightBack;

  /* Add UltraSonic Sensors to the manager */
  ultraSonicMgr.addSensor(pinFUltraTrig, pinFUltraEcho);
  ultraSonicMgr.addSensor(pinLUltraTrig, pinLUltraEcho);
  ultraSonicMgr.addSensor(pinRUltraTrig, pinRUltraEcho);

  mag.init();

  /* Outputs */
  wheels.init();
  arm.init();

  /*--- Add Runable Modules to Cycle Units ---*/
  act.addTask(&wheels, rate50Hz, 1);
  act.addTask(&arm, rate50Hz, 0);
  act.addTask(&heart, rate2Hz, 0);

  sense.addTask(&mag, rate250Hz, 0);
  sense.addTask(&lineManager, rate250Hz, 1);
  sense.addTask(&buttMan, rate100Hz, 0, "Button Manager");
  sense.addTask(&ultraSonicMgr, rate16Hz, 0);

  /*--- Initialize Cycle Units ---*/
  sense.setPrevMicro(micros());
  act.setPrevMicro(micros());
}

void loop()
{
    static bool light = true;

    //Sense
    sense.RunTasks(millis(),RS_LoadRings);

    //Plan
    plan.RunTasks(millis(),RS_LoadRings);

    //Act
    act.RunTasks(millis(),RS_LoadRings);

    //Act
    bullShitDemoCode();
}

void bullShitDemoCode(void) {
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

float driveSpeed = 15;

void followRightLine(void) {
    static bool buttFlag = false;
    static unsigned long buttTime = 0;

    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);

    if(backPair.valid){
        wheels.updateCommand(0,0,0);
    }
     else if(rightPair.valid){
            float adjustedAngleRad = rightPair.angle - PI / 2.0;
            float distanceToRight = 1.5;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad,driveSpeed);
            //Serial.println(adjustedAngleRad , 4);
            float xDistance = - 1 *(rightPair.offset.x - sensorCenters[LSL_RIGHT_FRONT].x );
            if(abs(xDistance) < distanceToRight * .5)
                xDistance = xDistance * .5 / distanceToRight;
            else {
                if(xDistance > 0)
                    xDistance = driveSpeed * .5;
                else
                    xDistance = -1 * driveSpeed * .5;
            }
            //float maxScale = xDistance + adjustedAngleRad;
            wheels.updateCommand(driveSpeed,xDistance,adjustedAngleRad);
            }
    else {
                wheels.updateCommand(0,0,0);
            }

    if (buttMan.getButtons() == 0b11) {
        if (!buttFlag) buttTime = millis();
        wheels.updateCommand(0,0,0);
        driveSpeed = driveSpeed + 10;
        delay(200);
        buttFlag = true;
    }

    if (buttFlag && (millis()-buttTime > 2000)) {
       //state = REVERSE_IT;
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

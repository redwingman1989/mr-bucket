#include "Globals.h"


void bullShitDemoCode(void);
void followRightLine(void);
void reverseIt(void);
void uTurn(void);
void moveToGetRings1(void);
void findLine(void);
void reverseItFork(void);
void moveToRightRecieve(void);
void moveToGetRings2(void);

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
  act.addTask(&wheels, rate50Hz, 0);
  act.addTask(&arm, rate50Hz, 0);
  act.addTask(&heart, rate2Hz, 0);

  sense.addTask(&mag, rate250Hz, 0);
  sense.addTask(&lineManager, rate250Hz, 0);
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

enum states{
    FOLLOW_RIGHT_LINE,
    MOVE_TO_GET_RINGS1,
    REVERSE_IT,
    U_TURN,
    FINDLINE,
    MOVETORIGHT_RECIEVE,
    REVERSE_IT_TO_FORK,
    MOVE_TO_GET_RINGS2
};


int state = FOLLOW_RIGHT_LINE;
int prevState = 0;
int stateChange = true;
uint16_t desiredHeading;

void bullShitDemoCode(void) {
  switch (state) {
    case FOLLOW_RIGHT_LINE:
        followRightLine();
        Serial.println("1");
        break;
    case MOVE_TO_GET_RINGS1:
        moveToGetRings1();
        Serial.println("2");
        break;
    case MOVE_TO_GET_RINGS2:
        moveToGetRings2();
        Serial.println("3");
        break;
    case REVERSE_IT:
        reverseIt();
        Serial.println("4");
        break;
    case REVERSE_IT_TO_FORK:
        reverseItFork();
        Serial.println("5");
        break;
    case MOVETORIGHT_RECIEVE:
        moveToRightRecieve();
        Serial.println("6");
        break;
    case FINDLINE:
        findLine();
        Serial.println("7");
        break;
    case U_TURN:
        uTurn();
        Serial.println("8");
        break;
  }

    if (prevState != state) stateChange = true;
    else stateChange = false;
    prevState = state;
}

void findLine(void){
    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    wheels.updateCommand(0,-5,0);
    if(rightPair.valid){
        state = FOLLOW_RIGHT_LINE;
    }
}

void followRightLine(void) {
    static int nextStatecounter = 0;
    float driveSpeed = 25;
    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);

    float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();

    if((backPair.valid && distanceToFront < 30) || distanceToFront < 5){
        wheels.updateCommand(0,0,0);
        nextStatecounter ++;
        if(nextStatecounter > 100){
           state =  MOVE_TO_GET_RINGS1;
           nextStatecounter = 0;
        }
    }
     else if(rightPair.valid){
            float adjustedAngleRad = rightPair.angle - PI / 2.0;
            float distanceToRight = 1.2;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad,driveSpeed);
            //Serial.println(adjustedAngleRad , 4);
            float xDistance = - 1 *(rightPair.offset.x - sensorCenters[LSL_RIGHT_FRONT].x );
            //Serial.println(xDistance , 4);
            if(abs(xDistance) < distanceToRight * .5){
              xDistance = xDistance * .5 / distanceToRight * 15;
            }
            else {
                if(xDistance > 0)
                    xDistance = 15 * .25;
                else
                    xDistance = -1 * 15 * .25;
            }

            if(distanceToFront < 30)
                wheels.updateCommand(driveSpeed * distanceToFront / 94 + 5 ,xDistance,adjustedAngleRad);
            else
                wheels.updateCommand(driveSpeed ,xDistance,adjustedAngleRad);
            }
            else {
                wheels.updateCommand(0,0,0);
            }


}


void moveToGetRings1(){
    float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();

    if(buttMan.getButtons() == 0b10 || buttMan.getButtons() == 0b01 || distanceToFront < 1){
        wheels.updateCommand(0,0,0);
        desiredHeading = mag.getRawHead() - 180;
        delay(200);
        state = REVERSE_IT_TO_FORK;
    }
    else {
        wheels.updateCommand(15,0,0);
    }
}

void moveToGetRings2(){
    float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();

    if(buttMan.getButtons() & 0x3  || distanceToFront < 1){
        wheels.updateCommand(0,0,0);
        desiredHeading = mag.getRawHead() - 180;
        delay(200);
        state = REVERSE_IT;
    }
    else {
        wheels.updateCommand(15,0,0);
    }

}

void moveToRightRecieve(void){
    static int nextStatecounter = 0;
    float driveSpeed = 15;
    lineDriveCommand_t rightPair = lineManager.getLineDriveCommand(LSP_RIGHT);
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);

    float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();

    if(rightPair.valid ){
        wheels.updateCommand(0,0,0);
        nextStatecounter ++;
        if(nextStatecounter > 10){
           state =  MOVE_TO_GET_RINGS2;
           nextStatecounter = 0;
        }
    }
     else if(backPair.valid){
            float adjustedAngleRad = backPair.angle - PI / 2.0;
            float distanceToBack = 1.02;
            adjustedAngleRad = convertRadToPercent(adjustedAngleRad,driveSpeed);
            //Serial.println(adjustedAngleRad , 4);
            float yDistance = - 1 *(backPair.offset.y - sensorCenters[LSL_CENTER_BACK].y );
            //Serial.println(xDistance , 4);
            if(abs(yDistance) < distanceToBack * .5){
              yDistance = yDistance * .5 / distanceToBack * 15;
            }
            else {
                if(yDistance > 0)
                    yDistance = 15 * .25;
                else
                    yDistance = -1 * 15 * .25;
            }
            wheels.updateCommand(yDistance ,driveSpeed,adjustedAngleRad);
     }
}

void reverseItFork(void){
    static int nextStatecounter = 0;
    lineDriveCommand_t backPair = lineManager.getLineDriveCommand(LSP_BACK);
    wheels.updateCommand(-25,0,0);

    if(backPair.valid){
    wheels.updateCommand(0,0,0);
       nextStatecounter++;
        if(nextStatecounter > 10){
            state = MOVETORIGHT_RECIEVE;
            nextStatecounter = 0;
        }
    }

}

void reverseIt(void) {
    static int nextStatecounter = 0;
    float distanceToFront = ultraSonicMgr.getSensor(FRONT)->calculateDistance();

    wheels.updateCommand(-25,0,0);
    if ( distanceToFront > 15){
        if(nextStatecounter++ > 100){
           state = U_TURN;
           nextStatecounter = 0;
        }
    }
}

void uTurn(void) {
    int16_t heading = mag.getRawHead();
    int16_t delta;
    static int16_t lastDelta = 0;
    int16_t deltaDelta;


    delta = desiredHeading - heading;
    if (delta > 180) delta -= 360;
    if (delta < -180) delta += 360;
    deltaDelta = delta - lastDelta;


    if (abs(delta) <= 3 && abs(deltaDelta) < 3) {
        state = FINDLINE;
        wheels.updateCommand(0,0,0);
    }
    else {
        if (delta>0) wheels.updateCommand(0,0,(0.2*delta));
        else wheels.updateCommand(0,0,(0.2*delta));
    }
    lastDelta = delta;
}

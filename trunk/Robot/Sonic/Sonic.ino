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
void followCenterLine(void);
void scoreRings(void);
void backupForGetRings(void);
void uturnToGetRings(void);
void findLineRight(void);

void setup()
{
  Serial2.begin(serialBaud);

  /*--- Initialize Runable Modules ---*/
  /* Inputs */
  buttMan.init();
  buttMan.addButton(pinLButton); // Button 0
  buttMan.addButton(pinRButton); // Button 1

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
  sense.addTask(&mag, rate250Hz, 0);
  sense.addTask(&lineManager, rate250Hz, 0);
  sense.addTask(&buttMan, rate100Hz, 0, "Button Manager");
  sense.addTask(&ultraSonicMgr, rate16Hz, 0);

  plan.addTask(&masterChief, rate100Hz, 0);

  act.addTask(&wheels, rate100Hz, 0);
  act.addTask(&arm, rate50Hz, 0);

  /*--- Initialize Cycle Units ---*/
  sense.setPrevMicro(micros());
  plan.setPrevMicro(micros());
  act.setPrevMicro(micros());
}

void loop()
{
    //Sense
    sense.RunTasks(millis(),RS_LoadRings);

    //Plan
    plan.RunTasks(millis(),RS_LoadRings);

    //Act
    act.RunTasks(millis(),RS_LoadRings);

 }

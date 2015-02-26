#include "Globals.h"
#include "System\CycleUnit.h"
#include "Setup.h"
#include "UltraSonicManager.h"
#include "UltraSonicSensor.h"
#include "UltraSonic_ino_header.h"

UltraSonicManager ultraSonicMgr = UltraSonicManager();
UltraSonicSensor * ptrSensor;
static float distance = 0;

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial(115200);
  setupPinModes();
  setupUltraSonicInterrupts();

  /* Add UltraSonic Sensors to the manager */
  ultraSonicMgr.addSensor(pinFUltraTrig, pinFUltraEcho);
  ultraSonicMgr.addSensor(pinLUltraTrig, pinLUltraEcho);
  ultraSonicMgr.addSensor(pinRUltraTrig, pinRUltraEcho);

  /* Add the UltraSonic Manager to the list of tasks to run. */
  sense.addTask(&ultraSonicMgr, rate16Hz, false);

  ptrSensor = ultraSonicMgr.getSensor(FRONT);
}


/********************************************************************
 Function: void loop(void)
 Description: Arduino main loop function. Runs continuously. Divided
  up into interrupt drven functions, including periodic, and interrupt
  dirven functions.
********************************************************************/
void loop() {
  //Sense
  sense.RunTasks(millis(),RS_LoadRings);

  /* Calculate distance and display it */

  if (ptrSensor->getTimeForCalcFlag() == true) {
    Serial.print("Start: ");
    Serial.println(ptrSensor->getFirstEchoTime());
    Serial.print("End: ");
    Serial.println(ptrSensor->getLastEchoTime());
    distance = ptrSensor->calculateDistance();
    Serial.print("Distance: ");
    Serial.println(distance);
    ptrSensor->setReadyForDistanceCalc(false);
  }
}


/****************************************************************
 * Function: void cycle(void)
 * Description: Called from loop() at 50Hz. Handles periodic tasks
 *                at 50Hz including:
 *                  - Calling the Line Sensor's Exec Function
****************************************************************/
void cycle() {
  /* Pulse Heartbeat LED on PIN 13 */
  heartbeat();


}


/************************************************************
 Function: heartbeat()
 Description: I'm not explaining this.
************************************************************/
void heartbeat() {
  // Toggle Board LED at 4 Hz
//  if (!(icount % 25))
//    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}


/****************************************************************
 Function: void PIT(void)
 Description: Function called at expiration of the periodic timer
 at 50Hz (every 20ms). This function sets the flags associated
 with periodic tasks to be executed form loop() and cycle().
*****************************************************************/
void PIT() {
  //static unsigned int scaler = 0;
  /* ******* ADJUST THIS ******* */
  //if (iFlags.pit_100Hz) {
    //sprintf(outStr, "cycle overrun");
    //Serial.println(outStr);
  //}
//  iFlags.pit_50Hz = 1;
  //if(++scaler % 10 == 0) {
   // iFlags.pit_50Hz = 1;
    //if(scaler == 20) {
     // iFlags.pit_25Hz = 1;
      //scaler = 0;
   // }
  //}
}

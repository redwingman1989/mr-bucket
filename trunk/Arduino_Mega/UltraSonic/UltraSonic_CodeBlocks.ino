#include <Arduino.h>
#include "System\CycleUnit.h"
#include "Setup.h"
#include "UltraSonicManager.h"
#include "UltraSonicSensor.h"
#include "UltraSonic_ino_header.h"

CycleUnit sense;
CycleUnit plan;
CycleUnit act;

UltraSonicSensor ultraSonicFront(frontUltraSonic);
UltraSonicSensor ultraSonicLeft(leftUltraSonic);
UltraSonicSensor ultraSonicRight(rightUltraSonic);
UltraSonicSensor * ultraSonicArr[NUM_ULTRA_SENSORS];
UltraSonicManager ultraSonicMgr(ultraSonicArr);


#define LED_PIN (13)

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial(115200);
  setupPinModes();
  setupUltraSonicInterrupts();

  ultraSonicArr[FRONT] = &ultraSonicFront;
  ultraSonicArr[LEFT]  = &ultraSonicLeft;
  ultraSonicArr[RIGHT] = &ultraSonicRight;

  sense.addTask(&ultraSonicMgr);

}


/********************************************************************
 Function: void loop(void)
 Description: Arduino main loop function. Runs continuously. Divided
  up into interrupt drven functions, including periodic, and interrupt
  dirven functions.
********************************************************************/
void loop() {
  /* Debug Varibales*/
//  static uint32_t loopCount = 0;
//  static bool lockOutPulse = false;
//  static UltraSonicSensor * ptrSensor = &ultraSonicFront;
//
//  if (lockOutPulse  == false) {
//    Serial.println("Dbg Flag 1");
//    ptrSensor->triggerAPulse();
//    lockOutPulse = true;
//  }
//  if (loopCount % 100000) {
//    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
//  }
//
//  if (ptrSensor->getTimeForCalcFlag() == true) {
//    Serial.println("Dbg Flag 2");
//    ptrSensor->setReadyForDistanceCalc(false);
//    lockOutPulse = false;
//    Serial.print("Reading: ");
//    Serial.println(ptrSensor->calculateDistance());
//  }
//
//  ++loopCount;
  //Sense
  sense.RunTasks(millis(),RS_LoadRings);
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

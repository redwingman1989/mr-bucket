#include <stdio.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include "global.h"
#include "lineSensor.h"

unsigned char runLoopFlag = 1;
unsigned long time = 0;
unsigned long timeDelta = 0;
unsigned char dbgLoopIterator = 0;

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial();
  setupTimer1Int();
  setupTimer3Int();
  setupLineSensor();
  setupPinModes();
}


/********************************************************************
 Function: void loop(void)
 Description: Arduino main loop function. Runs continuously. Divided
  up into interrupt drven functions, including periodic, and interrupt
  dirven functions.
********************************************************************/
void loop() {
  /* Log the Arduino Loop starting time in milliseconds */
  loopStartTime = millis();
  
  /* Execute cycle() at 100Hz */
  if (iFlags.pit_100Hz == 1) {
    /* Execute Main Cycle */
    cycle();
    /* Update the Cycle Count */
    icount++;
    /* Reset the 100Hz Flag */
    iFlags.pit_100Hz = 0;
  }
  
  /* Debug printout for Line Sensor Readings */
  if (DEBUG == 1 && ((icount % 20) == 0)) {
    for (dbgLoopIterator = 0; dbgLoopIterator < NUM_LINE_SENSOR_SENSORS; dbgLoopIterator++) {
      if (frontLineSensorDischargeTimes[dbgLoopIterator] == BLACK) {
        Serial.println(dbgLoopIterator);
      }
    }
  }
  
}


/****************************************************************
 Function: void cycle(void)
 Description: Called from loop() at 100Hz. Handles periodic tasks
 at 100Hz including:
   - Calling the Line Sensor's Exec Function
****************************************************************/
void cycle() {
  /* Pulse Heartbeat LED on PIN 13 */
  heartbeat(); 
  
  /* Run Exec for the Line Sensor */
  lineSensorExec();
  
}


/************************************************************ 
 Function: heartbeat()
 Description: I'm not explaining this.
************************************************************/
void heartbeat() {
  // Toggle Board LED at 4 Hz
  if (!(icount % 50))
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); 
}


/****************************************************************
 Function: void PIT(void)
 Description: Function called at expiration of the periodic timer
 at 100 Hz (every 10ms). This function sets the flags associated
 with periodic tasks to be executed form loop() and cycle().
*****************************************************************/
void PIT() {
  //static unsigned int scaler = 0;
  /* ******* ADJUST THIS ******* */
  //if (iFlags.pit_100Hz) {
    //sprintf(outStr, "cycle overrun");
    //Serial.println(outStr); 
  //}
  iFlags.pit_100Hz = 1;
  //if(++scaler % 10 == 0) {
   // iFlags.pit_50Hz = 1;
    //if(scaler == 20) {
     // iFlags.pit_25Hz = 1;
      //scaler = 0; 
   // }
  //}
}

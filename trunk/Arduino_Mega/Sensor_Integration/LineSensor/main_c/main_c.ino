#include <stdio.h>
#include <TimerOne.h>
#include <TimerThree.h>
#include "global.h"

/* Preprocessor Debug indicator. change to 1 for debug build */
#define DEBUG 1

unsigned char runLoopFlag = 1;
unsigned long time = 0;
unsigned long timeDelta = 0;

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial();
  setupTimer1Int();
  setupTimer3Int();
  setupLineSensor();
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
  
  if (runLoopFlag) {
    /* Local Variables */
    timeDelta = micros() - time;
    
    Serial.println(timeDelta);
    
    /* Reset the global run flag */
    runLoopFlag = 0;
    
    /* Charge the pin high */
    digitalWrite(LED_ON_PIN, HIGH);
    
    /* Make the Digital I/O Pin an Output */
    pinMode(SENSOR_1_PIN, OUTPUT);
  
    /* Set the Digital Out High */
    digitalWrite(SENSOR_1_PIN, HIGH);
  
    /* Delay for 20 microseconds */
    delayMicroseconds(20);
    
    /* Set Digital Pin Low */
    //digitalWrite(SENSOR_1_PIN, LOW);
  
    /* Make the Digital I/O pin an Input */
    pinMode(SENSOR_1_PIN, INPUT);
  
    /* Attach the Interrupt to the Falling signal on Int. 5 pin */
    attachInterrupt(5, LineSensorISR, FALLING);
  
    /* Start Timer */
    time = micros();
  
    
    /* Enable Interrupts */
    //interrupts();
  }
}


void LineSensorISR () {
  /* Local variables */
  
  
  /* Calculate the time */

  
   detachInterrupt(5);
   //digitalWrite(LED_ON_PIN, LOW);
  /* Disable Interrupts */
 // noInterrupts();
  
  /* Reset the global run loop flag */
  runLoopFlag = 1;
}


/****************************************************************
 Function: void cycle(void)
 Description: Called from loop() at 50Hz. handles periodic tasks
 at 50Hz including:
   - Calling the Line Sensor's Exec Function
****************************************************************/
void cycle() {
  /* Pulse Heartbeat LED on PIN 13 */
  heartbeat(); 
  
  /* Run Exec for the Line Sensor */
  lineSensorExec();
  
}


/****************************************************************
 Function: void PIT(void)
 Description: Function called at expiration of the periodic timer
 at 100 Hz (every 10ms). This function sets the flags associated
 with periodic tasks to be executed form loop() and cycle().
*****************************************************************/
void PIT() {
  static unsigned int scaler = 0;
  /* ******* ADJUST THIS ******* */
  if (iFlags.pit_50Hz) {
    sprintf(outStr, "cycle overrun");
    Serial.println(outStr); 
  }
  iFlags.pit_100Hz = 1;
  if(++scaler % 10 == 0) {
    iFlags.pit_50Hz = 1;
    if(scaler == 20) {
      iFlags.pit_25Hz = 1;
      scaler = 0; 
    }
  }
}

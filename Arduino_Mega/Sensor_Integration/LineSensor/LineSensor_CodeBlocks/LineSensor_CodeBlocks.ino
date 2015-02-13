#include <Arduino.h>
#include "../../../../../../ArduinoDev/arduino-1.0.6-windows/arduino-1.0.6/libraries/TimerOne/TimerOne.h"
#include "global.h"
#include "LineSensor.h"
#include "Setup.h"
#include "LineSensor_ino_header.h"

LineSensor* centerFrontLineSensor = new LineSensor(centerFront);
LineSensor* sideFrontLineSensor = new LineSensor(sideFront);
LineSensor* centerBackLineSensor = new LineSensor(centerBack);
LineSensor* sideBackLineSensor = new LineSensor(sideBack);

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial(SERIAL_BAUD_RATE);
  setupTimer1Int(PIT_PERIOD_IN_MICROSECS);
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
  /* Debug Varibales*/
  int i = 0;

  /* Log the Arduino Loop starting time in milliseconds */
  loopStartTime = millis();

  /* Execute cycle() at 50Hz */
  if (iFlags.pit_50Hz == 1) {
    /* Execute Main Cycle */
    cycle();
    /* Update the Cycle Count */
    icount++;
    /* Reset the 50Hz Flag */
    iFlags.pit_50Hz = 0;

  } /* if (PIT Flag) */
}


/****************************************************************
 Function: void cycle(void)
 Description: Called from loop() at 50Hz. Handles periodic tasks
 at 50Hz including:
   - Calling the Line Sensor's Exec Function
****************************************************************/
void cycle() {
  /* Pulse Heartbeat LED on PIN 13 */
  heartbeat();

  /* Prepare the line sensor to give a reading */
//  centerFrontLineSensor->beginCheck();
//  sideFrontLineSensor->beginCheck();
  centerBackLineSensor->beginCheck();
//  sideBackLineSensor->beginCheck();

  /* delay 300 us */
  delayMicroseconds(300);

//  centerFrontLineSensor->takeReading();
//  sideFrontLineSensor->takeReading();
  centerBackLineSensor->takeReading();
//  sideBackLineSensor->takeReading();

  for (int i = 0; i < 8; i++) {
    Serial.print((centerBackLineSensor->getLineSensorReadings() >> i) & 0x01);
    Serial.print("  ");
    if (i == 7) {
        Serial.println();
    }
  }
  //Serial.println(centerFrontLineSensor->getLineSensorReadings(), BIN);
}


/************************************************************
 Function: heartbeat()
 Description: I'm not explaining this.
************************************************************/
void heartbeat() {
  // Toggle Board LED at 4 Hz
  if (!(icount % 25))
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
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
  iFlags.pit_50Hz = 1;
  //if(++scaler % 10 == 0) {
   // iFlags.pit_50Hz = 1;
    //if(scaler == 20) {
     // iFlags.pit_25Hz = 1;
      //scaler = 0;
   // }
  //}
}

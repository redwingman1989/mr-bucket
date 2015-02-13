#include <Arduino.h>
#include "global.h"
#include "Setup.h"
#include "UltraSonicSensor.h"
#include "UltraSonic_ino_header.h"
#include "Interrupts.h"

UltraSonicSensor * ultraSonicFront = new UltraSonicSensor(frontUltraSonic);

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  setupSerial(SERIAL_BAUD_RATE);
  //setupTimer1Int(PIT_PERIOD_IN_MICROSECS);
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
  unsigned char * aPtr = (unsigned char *)0x22;
  unsigned int temp = (unsigned int)&PORTA;


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

  PORTA = 0xA5;
  Serial.println(PORTA);
  *aPtr = 0x2B;
  Serial.println(PORTA);
//  Serial.println(temp);
//  Serial.println(&PINA, HEX);
//  Serial.println(&PORTA, HEX);
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

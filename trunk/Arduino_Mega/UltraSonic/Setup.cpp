#include <Arduino.h>
#include "Interrupts.h"
#include "UltraSonicISR.h"
#include "UltraSonic_ino_header.h"
#include "UltraSonicSensor.h"

// /*******************************************************************
// Function: void setupTimer1Int(void)
// Description: Initializes the main PIT Interrupt Timer. The Timer
//                expires after periodInMicroSecs microseconds. When
//                the time expires the PIT function is called. The
//                timer may be configured using the
//                PIT_PERIOD_IN_MICROSECS macro, located in global
//                When the timer expires the PIT fuction is called as
//                the ISR.
//*******************************************************************/
//void setupTimer1Int(unsigned long periodInMicroSecs) {
//  Timer1.initialize(periodInMicroSecs);
//  Timer1.attachInterrupt(PIT);
//}


/*******************************************************************
 * Function: void setupUltraSonicInterrups(void)
 * Description: Initializes the interrupts for the UltraSonic Sensors.
 *******************************************************************/
void setupUltraSonicInterrups() {
    /* This is hard coded and I do not like it but for now I don't have much of
     *   an option, since I cannot set an ISR to a class method. */
  attachInterrupt(ULTRA_FRONT_EXT_INTERRUPT_NUM, frontUltraSonicISR, CHANGE);
  attachInterrupt(ULTRA_LEFT_EXT_INTERRUPT_NUM, leftUltraSonicISR, CHANGE);
  attachInterrupt(ULTRA_RIGHT_EXT_INTERRUPT_NUM, rightUltraSonicISR, CHANGE);
}


/*******************************************************************
 Function: void setupSerial(void)
 Description: Initializes the main debug serial port. The port is
                initialized to 9600 baud.
*******************************************************************/
void setupSerial(unsigned long baud) {
  Serial.begin(baud);
}


/*******************************************************************
 Function: void setupPinModes(void)
 Description: Initializes Arduino Mega Digital I/O pins.
*******************************************************************/
void setupPinModes() {
  pinMode(13, OUTPUT);
}

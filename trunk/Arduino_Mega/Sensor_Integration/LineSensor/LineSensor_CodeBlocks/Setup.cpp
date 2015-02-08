#include <Arduino.h>
#include "../../../../../ArduinoDev/arduino-1.0.6-windows/arduino-1.0.6/libraries/TimerOne/TimerOne.h"
//#include "TimerOne.h"
#include "LineSensor_ino_header.h"
#include "global.h"

/*******************************************************************
 Function: void setupTimer1Int(void)
 Description: Initializes the main PIT Interrupt Timer. The Timer
                expires after periodInMicroSecs microseconds. When
                the time expires the PIT function is called. The
                timer may be configured using the
                PIT_PERIOD_IN_MICROSECS macro, located in global
                When the timer expires the PIT fuction is called as
                the ISR.
*******************************************************************/
void setupTimer1Int(unsigned long periodInMicroSecs) {
  Timer1.initialize(periodInMicroSecs);
  Timer1.attachInterrupt(PIT);
}

/*******************************************************************
 Function: void setupTimer3Int(void)
 Description: Initializes the Line Sensor Polling Interrupt Timer.
                The Polling interrupt will be enabled and disabled
                during code execution. It will only be enabled after
                the capacitors have been charged. After the line is
                found the polling interrupt is disabled. The timer
                may be configured using the POLL_PERIOD_IN_MICROSECS
                macro, located in global.h
*******************************************************************/
void setupTimer3Int(unsigned long periodInMicroSecs) {
//  Timer3.initialize(periodInMicroSecs);
//  Timer3.attachInterrupt(pollLineSensor);
//  Timer3.stop();

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
 Function: void setupLineSensor(void)
 Description: Initializes the robot's Line Sensor. Set the LED Enable
                Digital I/O to an output. Set the LED Enable pin High.
*******************************************************************/
void setupLineSensor() {
  pinMode(ALL_LN_SEN_LED_ENABLE_PIN, OUTPUT);
  digitalWrite(ALL_LN_SEN_LED_ENABLE_PIN, HIGH);
}


/*******************************************************************
 Function: void setupPinModes(void)
 Description: Initializes Arduino Mega Digital I/O pins.
*******************************************************************/
void setupPinModes() {
  pinMode(LED_PIN, OUTPUT);
}

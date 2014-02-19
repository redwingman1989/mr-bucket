#include <stdio.h>
#include <TimerThree.h>
#include "global.h"
#include "crc.h"
#include "irprox.h"

#define DEBUG 1

void setup() { 
  crcInit();
  setupTimerInt();
  setupSerial();
  setupPinModes();
  setupMagnetometer();
  //setupUltraSonicInt();
  //messageInit(RIGHT);
  //messageInit(LEFT);
}

void loop() {
  if (iFlags.pit == 1) {  
    cycle();
    iFlags.pit = 0;      
  } 
/*  
  if (iFlags.ultraR == 1) {
    iFlags.ultraR = 0;
    updateMessageBuf(RIGHT);
    iFlags.sendR = 1;
  }
  if (iFlags.ultraL == 1) {
    iFlags.ultraL = 0;
    updateMessageBuf(LEFT);
    iFlags.sendL = 1;
  }
*/
  
}

void cycle() {
  cycleStartTime = millis();

  heartbeat();
  
  if (icount % 4 == 0)
    i2cExec();

  icount++;
}

void heartbeat() {
  // Toggle Board LED at 2 Hz
  if (!(icount % 25))
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); 
}

void printCycleTime() {
 // sprintf(outStr, "cycle time: %lu ms", (millis() - cycleStartTime));
 // Serial.println(outStr); 
}

void PIT() {
  if (iFlags.pit) {
    sprintf(outStr, "cycle overrun");
    Serial.println(outStr); 
  }  
  iFlags.pit = 1;
}


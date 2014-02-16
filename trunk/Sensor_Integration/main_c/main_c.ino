#include <stdio.h>
#include <TimerOne.h>
#include "global.h"
#include "crc.h"
#include "irprox.h"

#define DEBUG 1

void setup() { 
  crcInit();
  setupTimerInt();
  setupSerial();
  setupPinModes();
  setupUltraSonicInt();
  messageInit(RIGHT);
  messageInit(LEFT);
}

void loop() {
  if (iFlags.pit == 1) {  
    cycle();
    iFlags.pit = 0;      
  } 
  //if (iFlags.uartRx == 1 ) {
  //  iFlags.uartRx = 0;
  //  processMessage_Matt();
  //}
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
}

void cycle() {
  cycleStartTime = millis();

  heartbeat();
  
  if (iFlags.sendR && (icount & 0x01))
    transmitMessage(RIGHT);
  else if (iFlags.sendL && !(icount & 0x01))
    transmitMessage(LEFT);  
  
  trigEcho((icount & 0x01));
  
//  getIRDistanace();

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


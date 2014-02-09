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
}

void loop() {
  if (iFlags.pit == 1) {  
    cycle();
    iFlags.pit = 0;      
  } 
  
  if (iFlags.uartRx == 1 ) {
    iFlags.uartRx = 0;
    processMessage_Matt();
  }
  
  if (iFlags.ultraR == 1) {
    iFlags.ultraR = 0;
    rUltraDistance = rUltraTime/148.0;
    Serial.print("UltraSonic Distance (inches): "); 
    Serial.print(rUltraDistance);
    Serial.write('\n');
  }
}

void cycle() {
  cycleStartTime = millis();
  
  heartbeat();
  
  if((icount % 50) == 10)
   trigEchoRight();
  
  getIRDistanace();
  if ((icount % 50)== 0){
    printDistance();
  
    printCycleTime();
  }
 
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



#include <Wire.h>
#include "Magnetometer.h"

/* create global MAgnetomer with I2C address 0x1E */
Magnetometer mag (0x1E);

void setup(){
  Serial.begin(57600);
  mag.init();
}

void loop(){
  delay(20);
  mag.calcHeading();
  Serial.println(mag.getFiltHead());
}


#include <Arduino.h>
#include "ServoArmController.h"

//ServoArmController servoArm();

Servo testServo;

void setup()
{
  Serial.begin(9600);
	pinMode(13, OUTPUT);
	testServo.attach(3);
}

void loop()
{
  int temp;
  static int command = 1500;
  static int shadowCommand = -1;
  static int increment = 10;
  static int shadowIncrement = -1;

  static bool led;

  while (Serial.available() > 0 )
  {
    temp = Serial.read();

    if (temp == 'r') command = 1500;
    else if (temp == '+') command += increment;
    else if (temp == '-') command -= increment;
    else if (temp == '*') increment *= 10;
    else if (temp == '/') increment /= 10;
    else;

    if (increment < 1) increment = 1;
    if (increment > 100) increment = 100;
    if (command > 2000) command = 2000;
    if (command < 1000) command = 1000;
  }

  testServo.writeMicroseconds(command);

  if (command != shadowCommand) {
    Serial.print("CMD = ");
    Serial.println(command);
    shadowCommand = command;
  }

  if (increment != shadowIncrement) {
    Serial.print("INC = ");
    Serial.println(increment);
    shadowIncrement = increment;
  }

  led = !led;
  digitalWrite(13, led);
  delay(100);
}

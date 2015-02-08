#include <Arduino.h>
#include <Servo.h>
#include "ServoArmController.h"

ServoArmController servoArm;

Servo testServo;

void setup()
{
  //delay(5000);
  Serial.begin(57600);
	pinMode(13, OUTPUT);
/*  this->pickupLeft.attach(38);
  this->pickupCenter.attach(39);
  this->pickupRight.attach(40);
  this->swingArm.attach(41);
*/
	testServo.attach(25);
   servoArm.init();
}

void loop()
{
  int temp;
  static int command = 1500;
  static int shadowCommand = -1;
  static int increment = 10;
  static int shadowIncrement = -1;

  static bool led;

  //servoArm.commandSwingArm(SA_UP);
  servoArm.exec();
  while (Serial.available() > 0 )
  {
    temp = Serial.read();

    if (temp == 'r') command = 1500;
    else if (temp == '+') command += increment;
    else if (temp == '-') command -= increment;
    else if (temp == '*') increment *= 10;
    else if (temp == '/') increment /= 10;
    else if (temp == '1') servoArm.commandSwingArm(SA_DOWN);
    else if (temp == '2') servoArm.commandSwingArm(SA_UP);
    else if (temp == '3') servoArm.commandPickupServo(PU_LEFT, PS_LETGO);
    else if (temp == '4') servoArm.commandPickupServo(PU_LEFT, PS_GRAB);
    else if (temp == '5') servoArm.commandPickupServo(PU_CENTER, PS_LETGO);
    else if (temp == '6') servoArm.commandPickupServo(PU_CENTER, PS_GRAB);
    else if (temp == '7') servoArm.commandPickupServo(PU_RIGHT, PS_LETGO);
    else if (temp == '8') servoArm.commandPickupServo(PU_RIGHT, PS_GRAB);
    else;

    if (increment < 1) increment = 1;
    if (increment > 100) increment = 100;
    if (command > 3000) command = 3000;
    if (command < 100) command = 100;
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
  delay(20);
}

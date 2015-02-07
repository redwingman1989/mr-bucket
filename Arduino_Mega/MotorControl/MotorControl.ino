#include <Arduino.h>
#include "MotorController.h"

MotorController myController;

void setup()
{
	pinMode(13, OUTPUT);
	myController.init();
}

void loop()
{
	static unsigned long start;
	static bool led = false;
  static int state;

  //Blink led for status indicator
  led = !led;
	digitalWrite(13, led);

  //Set 100% fwd, 50% right, 25% clockwise
  myController.updateCommand(100,50,25);
  myController.exec();

  //50Hz loop
  delay(20);
}

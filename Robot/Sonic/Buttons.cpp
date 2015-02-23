#include "Buttons.h"

Button::Button(void) {

};

bool Button::init(uint8_t addr) {
  pinAddr = addr;
  pinMode(pinAddr, INPUT);
};

uint8_t Button::getAddress() {
  return pinAddr;
};

void Button::update() {
  pos = digitalRead(pinAddr);

  if (prevPos == pos) {
    H2LTran = 0;
    L2HTran = 0;
  }
  else if (!prevPos && pos) {
    L2HTran = 1;
    L2Hcount++;
  }
  else if (prevPos && !pos) {
    H2LTran = 1;
  }

  prevPos = pos;
};

bool Button::getPos() {
  return pos;
};

uint8_t Button::getTrans(){
  return ((buttonStatus >> 2) & 0x03);
};

uint8_t Button::getCount() {
  return ((buttonStatus >> 4) & 0x0F);
};

ButtonManager::ButtonManager(){

};

bool ButtonManager::init(){
  numButtons = 0;
  return true;
};

int8_t ButtonManager::addButton(uint8_t addr){
  int8_t result = -1;

  if (numButtons < MAX_BUTTONS-1) {
    result = buttons[numButtons++].init(addr);
  };

  return result;
};

void ButtonManager::readButtons(){
  int8_t i;
  uint8_t pos;
  uint8_t result = 0;

  for(i=(numButtons-1); i >= 0; i--) {
    buttons[i].update();
    pos = buttons[i].getPos();
    result |= (pos << i);
  };
  buttPoses = result;
};

bool ButtonManager::RunTick(uint16_t time,RobotState state) {
  readButtons();
  return true;
};


void ButtonManager::DebugOutput(HardwareSerial *serialPort) {
  serialPort->println(buttPoses);
};

uint8_t ButtonManager::getButtons(){
  return buttPoses;
};

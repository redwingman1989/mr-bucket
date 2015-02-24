#include "StateMachine.h"

template <class T> StateMachine<T>::StateMachine() {
}

template <class T> bool StateMachine<T>::RunTick() {
  stateChanged = (prevState != currentState);
  prevState = currentState;
  (this->*currentState)(stateChanged);
  return false;
}

template <class T> void StateMachine<T>::DebugOutput(HardwareSerial *serialPort) {
  serialPort->println("STATE MACHINE");
}



LEDStateMachine::LEDStateMachine() {
  currentState = (state) &LEDStateMachine::turnOn;
}

void LEDStateMachine::turnOn(bool changed) {
  Serial.println("ON");
  currentState = (state) &LEDStateMachine::turnOff;
}

void LEDStateMachine::turnOff(bool changed) {
  Serial.println("OFF");
  currentState = (state) &LEDStateMachine::turnOn;
}



LEDWrapper::LEDWrapper() {
  currentState = (state) &LEDWrapper::printHello;
}

void LEDWrapper::turnOnOffTwice(bool changed) {
  static int runCount;
  if (changed) runCount = 0;
  runCount++;
  ledMachine.RunTick();
  if (runCount == 4) currentState = (state) &LEDWrapper::printHello;
}

void LEDWrapper::printHello(bool changed) {
  Serial.println("HELLO");
  currentState = (state) &LEDWrapper::turnOnOffTwice;
}

#include "StateMachine.h"

//Constructor
template <class T> StateMachine<T>::StateMachine() {
}

//Default run tick function - run the current state
template <class T> bool StateMachine<T>::RunTick() {
  runCurrentState();
  return false;
}

//Default debug output function - print nothing
template <class T> void StateMachine<T>::DebugOutput(HardwareSerial *serialPort) {
  serialPort->println("lame default debug");
}

//Run the current state and update state machine members
template <class T> void StateMachine<T>::runCurrentState() {
  state temp = currentState;
  //Determine if the state just changed
  stateChanged = (prevState != currentState);

  //Execute the current state and pass in if the state just changed
  (this->*currentState)(stateChanged);

  //Update state shadow
  prevState = temp;
}



//Example usages of the state machine class

//Constructor for child class
LEDStateMachine::LEDStateMachine() {
  //Setup default state in the constructor
  currentState = (state) &LEDStateMachine::turnOn;
}

//Override the default debug output
void LEDStateMachine::DebugOutput(HardwareSerial *serialPort) {
  Serial.println("my custom runTick debug output");
}

//Override the default run tick
bool LEDStateMachine::RunTick() {
  Serial.println("my custom runTick logic");
  //Call this to execute the current state (or not, whatever you want)
  runCurrentState();
  return false;
}

//State definitions
//Print ON then change state to OFF
void LEDStateMachine::turnOn(bool changed) {
  Serial.println("ON");
  currentState = (state) &LEDStateMachine::turnOff;
}
//Print OFF then change state to ON
void LEDStateMachine::turnOff(bool changed) {
  Serial.println("OFF");
  currentState = (state) &LEDStateMachine::turnOn;
}


//Constructor for another child class
LEDWrapper::LEDWrapper() {
  //Setup default state
  currentState = (state) &LEDWrapper::printHello;
}
//Run the LEDStateMachine 4 ticks before advancing state
void LEDWrapper::turnOnOffTwice(bool changed) {
  static int runCount;
  if (changed) runCount = 0;
  runCount++;
  //Calling sub-state machine runTick to execute
  ledMachine.RunTick();
  ledMachine.DebugOutput(&Serial);
  if (runCount == 4) currentState = (state) &LEDWrapper::printHello;
}
//Print hello and then switch back to the other state
void LEDWrapper::printHello(bool changed) {
  Serial.println("HELLO");
  currentState = (state) &LEDWrapper::turnOnOffTwice;
}

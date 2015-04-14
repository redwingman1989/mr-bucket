#pragma once

#include "System/RunableModule.h"

template <class T>
class StateMachine : public RunableModule {
public:
  //Typedef for the state function pointers
  typedef void (StateMachine<T>::*state) (bool);

  //Constructor
  StateMachine<T>();

  //Default implementations of RunableModule virtuals
  bool RunTick();
  void DebugOutput(HardwareSerial *serialPort);

protected:
  //Run current state is called by RunTick to update private data and execute the current state
  void runCurrentState();

  //Boolean used to indicate that you have just entered a new state
  bool stateChanged;

  //Function pointers to the current and previous states
  state currentState;
  state prevState;
};
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
  //Determine if the state just changed
  stateChanged = (prevState != currentState);
  prevState = currentState;
  //Execute the current state and pass in if the state just changed
  (this->*currentState)(stateChanged);

}
//Constructor


//Example usages of the state machine class

////Each state machine will be an independent child class
//class LEDStateMachine : public StateMachine<LEDStateMachine> { //Note the class type in the <> of StateMachine
//public:
//  LEDStateMachine(); //Constructor
//  bool RunTick(); //Overriding default state machine runtick
//  void DebugOutput(HardwareSerial *serialPort); //Overriding default state machine debug
//private:
//  //My state functions
//  void turnOn(bool);
//  void turnOff(bool);
//};
//
//class LEDWrapper : public StateMachine<LEDWrapper> {
//public:
//  LEDWrapper(); //Constructor
//private:
//  //Heard you like state machines....
//  LEDStateMachine ledMachine;
//  //My state functions
//  void turnOnOffTwice(bool);
//  void printHello(bool);
//};

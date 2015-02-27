#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "System/RunableModule.h"

template <class T>
class StateMachine : public RunableModule {
public:
  //Typedef for the state function pointers
  typedef void (StateMachine<T>::*state) (bool);

  //Constructor
  StateMachine();

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

#endif // STATEMACHINE_H

//Example usages of the state machine class

//Each state machine will be an independent child class
class LEDStateMachine : public StateMachine<LEDStateMachine> { //Note the class type in the <> of StateMachine
public:
  LEDStateMachine(); //Constructor
  bool RunTick(); //Overriding default state machine runtick
  void DebugOutput(HardwareSerial *serialPort); //Overriding default state machine debug
private:
  //My state functions
  void turnOn(bool);
  void turnOff(bool);
};

class LEDWrapper : public StateMachine<LEDWrapper> {
public:
  LEDWrapper(); //Constructor
private:
  //Heard you like state machines....
  LEDStateMachine ledMachine;
  //My state functions
  void turnOnOffTwice(bool);
  void printHello(bool);
};

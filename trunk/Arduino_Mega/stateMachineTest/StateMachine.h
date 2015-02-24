#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "System/RunableModule.h"

template <class T>
class StateMachine : public RunableModule {
public:
  typedef void (StateMachine<T>::*state) (bool);
  StateMachine();
  bool RunTick();
  void DebugOutput(HardwareSerial *serialPort);

protected:
  bool stateChanged;
  state currentState;
  state prevState;
};

class LEDStateMachine : public StateMachine<LEDStateMachine> {
public:
  LEDStateMachine();
private:
  void turnOn(bool);
  void turnOff(bool);
};

class LEDWrapper : public StateMachine<LEDWrapper> {
public:
  LEDWrapper();
private:
  LEDStateMachine ledMachine;
  void turnOnOffTwice(bool);
  void printHello(bool);
};



#endif // STATEMACHINE_H

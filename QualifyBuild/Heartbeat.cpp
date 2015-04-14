#include "Heartbeat.h"

Heartbeat::Heartbeat(uint8_t addr) {
  pinAddr = addr;
};

bool Heartbeat::init() {
  pinMode(pinAddr, OUTPUT);
  return true;
};

bool Heartbeat::RunTick() {
  ledState = !ledState;
  digitalWrite(pinAddr, ledState);
}

void Heartbeat::DebugOutput(HardwareSerial* serialPort) {
  char output[50];
  sprintf(output, "ledState: %d", ledState);
  serialPort->println(output);
};

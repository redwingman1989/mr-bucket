
void rightEchoInt() {
  volatile static long tempTime;
  volatile static long startTime;
  tempTime = micros();
  if (digitalRead(ULTRA_ECHO_RPIN))
    startTime = tempTime;
  else {
    UltraTime[RIGHT] = (tempTime - startTime);
    iFlags.ultraR = 1;
  }
}

void leftEchoInt() {
  volatile static long tempTime;
  volatile static long startTime;
  tempTime = micros();
  if (digitalRead(ULTRA_ECHO_LPIN))
    startTime = tempTime;
  else {
    UltraTime[LEFT] = (tempTime - startTime);
    iFlags.ultraL = 1;
  }
}

void trigEcho(int side) {
  digitalWrite((ULTRA_TRIG_RPIN + side), HIGH);
  delayMicroseconds(10);
  digitalWrite((ULTRA_TRIG_RPIN + side), LOW);
}

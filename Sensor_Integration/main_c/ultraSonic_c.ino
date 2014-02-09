
void rightEchoInt() {
  volatile static long tempTime;
  volatile static long startTime;
  tempTime = micros();
  if (digitalRead(ULTRA_ECHO_RPIN))
    startTime = tempTime;
  else {
    rUltraTime = (tempTime - startTime);
    iFlags.ultraR = 1;
  }
}

void leftEchoInt() {
  volatile static long tempTime;
  volatile static long startTime;
  tempTime = micros();
  
  if (digitalRead(ULTRA_ECHO_LPIN))
    startTime = tempTime;
  else
    lUltraTime = (tempTime - startTime);
}

void trigEchoRight() {
  digitalWrite(ULTRA_TRIG_RPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIG_RPIN, LOW);
}

void trigEchoLeft() {
  digitalWrite(ULTRA_TRIG_LPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_TRIG_LPIN, LOW);
}

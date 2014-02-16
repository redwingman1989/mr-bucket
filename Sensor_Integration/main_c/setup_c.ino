void setupTimerInt() {
  Timer1.initialize(20000);
  Timer1.attachInterrupt(PIT);
}

void setupSerial() {
  Serial.begin(115200);
  Serial.setTimeout(15);
}

void setupPinModes() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(ULTRA_TRIG_LPIN, OUTPUT);
  pinMode(ULTRA_TRIG_RPIN, OUTPUT);
  pinMode(ULTRA_ECHO_LPIN, INPUT);
  pinMode(ULTRA_ECHO_RPIN, INPUT);
}

void setupUltraSonicInt() {
  attachInterrupt(ULTRA_RIGHT_ECHO_INT, rightEchoInt, CHANGE);
  attachInterrupt(ULTRA_LEFT_ECHO_INT, leftEchoInt, CHANGE);
}

void messageInit(uint8_t side) {
  int i = 0;
  message[side][i++] = 0xAA; 
  message[side][i++] = 0xAA; 
  message[side][i++] = side; 
  message[side][i++] = (DATA_SIZE - DATA_0); 
}

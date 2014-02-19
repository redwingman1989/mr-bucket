void setupTimerInt() {
  Timer1.initialize(2000);
  Timer1.attachInterrupt(PIT);
}

void setupRobot(robot * bot) {
  bot->Ultra_LeverArm[RIGHT] = ULTRA_RIGHT_LEVER_ARM;
  bot->Ultra_LeverArm[LEFT]  = ULTRA_LEFT_LEVER_ARM;
  bot->IR_LeverArm[RIGHT]    = ULTRA_RIGHT_LEVER_ARM;
  bot->IR_LeverArm[LEFT]     = ULTRA_LEFT_LEVER_ARM;
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

void messageInit(void) {
  int i = 0;
  message[i++] = 0xAA; 
  message[i++] = 0xAA; 
}

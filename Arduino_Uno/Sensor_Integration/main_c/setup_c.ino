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

void setupMagnetometer() {
  Wire.begin();
  magData.cfg_A.data_rate   = DR_15_HZ;
  magData.cfg_A.avg         = SA_1;
  magData.cfg_B.gain        = G_1_3_GA;
  magData.mode.mode_sel     = MM_CMM;
  SENDBYTE(MAG_ADDR, MR_CFG_REG_A, magBuffer[MR_CFG_REG_A]);
  SENDBYTE(MAG_ADDR, MR_CFG_REG_B, magBuffer[MR_CFG_REG_B]);
  SENDBYTE(MAG_ADDR, MR_MODE_REG, magBuffer[MR_MODE_REG]);
}

void messageInit(void) {
  int i = 0;
  message[i++] = 0xAA; 
  message[i++] = 0xAA; 
}



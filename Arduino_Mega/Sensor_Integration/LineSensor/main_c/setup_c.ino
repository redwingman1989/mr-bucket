/*******************************************************************
 Function: void setupTimer1Int(void)
 Description: Initializes the main PIT Interrupt Timer. The Timer
                expires after 20,000 microseconds (20 milliseconds).
                When the timer expires the PIT fuction is called as
                the ISR.                                   [50 Hz]
*******************************************************************/
void setupTimer1Int() {
  Timer1.initialize(20000);
  Timer1.attachInterrupt(PIT);
}

/*******************************************************************
 Function: void setupTimer3Int(void)
 Description: Initializes the Line Sensor Polling Interrupt Timer.
                The Polling interrupt will be enabled and disabled
                during code execution. It will only be enabled after
                the capacitors have been charged. After the line is
                found the polling interrupt is disabled. The Timer
                expires after 1000 microseconds (0.001 milliseconds).
                                                           [1000 Hz]
*******************************************************************/
void setupTimer3Int() {
  Timer3.initialize(1000);
  Timer3.attachInterrupt(pollLineSensor);
  Timer3.stop();
  
}

/*******************************************************************
 Function: void setupSerial(void)
 Description: Initializes the main debug serial port. The port is
                initialized to 9600 baud.
*******************************************************************/
void setupSerial() {
  Serial.begin(115200);
}

/*******************************************************************
 Function: void setupLineSensor(void)
 Description: Initializes the robot's Line Sensor. Set the LED Enable
                Digital I/O to an output. Set the LED Enable pin High.
*******************************************************************/
void setupLineSensor() {
  pinMode(ALL_LN_SEN_LED_ENABLE_PIN, OUTPUT);
  digitalWrite(ALL_LN_SEN_LED_ENABLE_PIN, HIGH); 
}


/*******************************************************************
 Function: void setupPinModes(void)
 Description: Initializes Arduino Mega Digital I/O pins.
*******************************************************************/
void setupPinModes() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 1, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 2, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 3, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 4, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 5, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 6, OUTPUT);
  pinMode(DBG_LIGHT_ARR_START_PIN + 7, OUTPUT);
}

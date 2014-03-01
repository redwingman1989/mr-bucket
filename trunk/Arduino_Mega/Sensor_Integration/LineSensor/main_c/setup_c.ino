/*******************************************************************
 Function: void setupTimer1Int(void)
 Description: Initializes the main PIT Interrupt Timer. The Timer
                expires after 10,000 microseconds (10 milliseconds).
                When the timer expires the PIT fuction is called as
                the ISR.                                   [100 Hz]
*******************************************************************/
void setupTimer1Int() {
  Timer1.initialize(10000);
  Timer1.attachInterrupt(PIT);
}

/*******************************************************************
 Function: void setupTimer3Int(void)
 Description: Initializes the Line Sensor Polling Interrupt Timer.
                The Polling interrupt will be enabled and disabled
                during code execution. It will only be enabled after
                the capacitors have been charged. After the line is
                found the polling interrupt is disabled. The Timer
                expires after 50 microseconds (0.05 milliseconds).
                                                           [20 kHz]
*******************************************************************/
void setupTimer3Int() {
  Timer3.initialize(50);
}

/*******************************************************************
 Function: void setupSerial(void)
 Description: Initializes the main debug serial port. The port is
                initialized to 9600 baud.
*******************************************************************/
void setupSerial() {
  Serial.begin(9600);
}

/*******************************************************************
 Function: void setupLineSensor(void)
 Description: Initializes the robot's Line Sensor. Set the LED Enable
                Digital I/O to an output. Set the LED Enable pin High.
*******************************************************************/
void setupLineSensor() {
  pinMode(LN_SEN_LED_ENABLE_PIN, OUTPUT);
  digitalWrite(LN_SEN_LED_ENABLE_PIN, HIGH); 
}

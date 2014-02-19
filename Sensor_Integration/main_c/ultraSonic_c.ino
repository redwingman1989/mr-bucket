/**********************************************************
 Function: rightEchoInt()
 Background: The ultrasonics set the echo pin high for the 
   duration of the time in microseconds the sound waves to 
   come back.
 Description: Attached to external interrupt on PIN 2. 
  Called upon transtion of external interrupt pin
  (low to high and high to low). If the pin is high record
  start time, if the pin is low calculate and store the duration
  in microseconds.
**********************************************************/
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
/*********************************************************
 Function: leftEchoInt()
 Decription: Attached to External interrupt on PIN 3. 
   see rightEchoInt()
*********************************************************/
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
/********************************************************
 Function: void trigEcho(int)
 Parameters: Interger of side (0 = RIGHT, 1 = LEFT)
 Description: Sets pulse high for 10 micro seconds on the
   side indicated in parameter. this triggers the sensor
   to pulse an ultra sonic wave.   
********************************************************/
void trigEcho(int side) {
  digitalWrite((ULTRA_TRIG_RPIN + side), HIGH);
  delayMicroseconds(10);
  digitalWrite((ULTRA_TRIG_RPIN + side), LOW);
}
/********************************************************
 Function: void trigEcho(robot, uint8_t side)
 Parameters: Interger of side (0 = RIGHT, 1 = LEFT)
 Description: Sets pulse high for 10 micro seconds on the
   side indicated in parameter. this triggers the sensor
   to pulse an ultra sonic wave.   
   
   Equation Explination:
     to get ultrasonic time in micro seconds to inches 
        time_ms/148
     X coordinates are in half inches, so
       time_ms/74
     to handle rounding to the nearest X, we need to add
     multiply time_ms and add 5, then divide by 740.
       ((time_ms*10)+5)/740
********************************************************/
void updateUltrasonic(uint8_t side) {
  /* Process Right Ultrasonic */
  if (side == RIGHT) {
    Robot.Ultra_X[RIGHT] = (uint8_t)(189 - ((((UltraTime[RIGHT]*10)+5)/740) + Robot.Ultra_LeverArm[RIGHT]));
  }
  /* Process Left Ultrasonic */
  else {
    Robot.Ultra_X[LEFT] = (uint8_t)((((UltraTime[LEFT]*10)+5)/740) + Robot.Ultra_LeverArm[LEFT]);
  }
}

//#include "lineSensor.h"



unsigned long startCapChargeTime = 0;

/****************************************************************
 Function: void lineSensorExec(void)
 Description: Called from loop() at 100Hz. Handles Line Sensor Functions:
   1) Turns on the IR Emitters on the Line Sensor
   2) Waits until the capacitors are charged (10 microseconds)
   3) Enables the Polling Interrupt (Runs at 50 microseconds)
****************************************************************/
void lineSensorExec() {
  unsigned char loopIterator = 0;
  /* Reset the timeArrays */
  for (loopIterator = 0; loopIterator < NUM_LINE_SENSOR_SENSORS; loopIterator++) {
    frontLineSensorDischargeTimes[loopIterator] = INDETERMINATE;
    rightLineSensorDischargeTimes[loopIterator] = INDETERMINATE;
    rearLineSensorDischargeTimes[loopIterator] = INDETERMINATE;
    leftLineSensorDischargeTimes[loopIterator] = INDETERMINATE;
  }
  
  /* Turn on IR Emitters to Charge Capacitors */
  digitalWrite(ALL_LN_SEN_LED_ENABLE_PIN, HIGH);
  
  /* Make the Digital I/O Pin an Output */
  pinMode(FRONT_LN_SEN_SENSOR_1_PIN, OUTPUT);
  
  /* Set the Digital Out High */
  digitalWrite(FRONT_LN_SEN_SENSOR_1_PIN, HIGH);
  
  /* Delay 10 microseconds to charge the capacitors. As the delay is only
   *   10 microseconds, we can afford to hold the processor for that long. */
  delayMicroseconds(10);  // ***** Need to verify this won't affect other Interrupts!!!! ****** //
  
  /* Turn off IR Emitters */
  digitalWrite(ALL_LN_SEN_LED_ENABLE_PIN, LOW);
  
  /* Make the Digital I/O pin an Input */
  pinMode(FRONT_LN_SEN_SENSOR_1_PIN, INPUT);
  
  /* Mark the Start time that the caps were charged */
  startCapChargeTime = millis();
  
  /* Enable the polling interrupt */
  Timer3.attachInterrupt(pollLineSensor);
  
}


/****************************************************************
 Function: void pollLineSensor(void)
 Description: Called by the expiration of Line Sensor Polling Interrupt
                Timer. This function gets readings of the capacitor
                discharge times and based on a threshold value, sets
                whether or not a specific sensor on the line sensor
                array sees White (No Line) or Black (Line).
                
                Sensor Check prioritization:
                  *** [FRONT] then [RIGHT] then [LEFT] then [REAR]: ***
                1) If the line hasn't been detected on the rear sensor,
                     check the front sensor.
                2) If the line hasn't been detected on the left sensor,
                     check the right sensor.
                3) If the line hasn't been detected on the right sensor,
                     check the left sensor.
                4) If the line hasn't been detected on the front sensor,
                     check the rear sensor.
                
                Discharge Times on surfaces (Function of Distance to line):
                White = ~100 microseconds 
                Black = 2000+ microseconds
****************************************************************/
void pollLineSensor() {
  unsigned long capDischargeTime = 0; // Temp Location to store Cap Discharge Time
  unsigned char i = 0;                // Loop Iterator
  
  /* If the line hasn't been detected under the rear sensor, check the front sensor for the line */
  if (lineUnderSensor.rear == FALSE) {
    /* Sample the Front Line Sensor */
    for (i = FRONT_LN_SEN_SENSOR_1_PIN; i < FRONT_LN_SEN_SENSOR_1_PIN + NUM_LINE_SENSOR_SENSORS; i++) {
      /* Check to see if the Line has already been found on this sensor */
      if (frontLineSensorDischargeTimes[i - FRONT_LN_SEN_SENSOR_1_PIN] == BLACK) {
        /* The Line was found on this Sensor, no need to check the whole sensor */
        lineUnderSensor.front = TRUE;
        
        /* Skip checking the rest of the sensor */
        break;
      }
      
      /* Check to see if the Pin is low. Mark the time it took to discharge
       *   the capacitor if the pin is low. */
      if (digitalRead(i) == LOW) {
        capDischargeTime = millis() - startCapChargeTime;
        if (capDischargeTime >= BLACK_LINE_MICROSEC_THRESHOLD_VALUE) {
          /* Flag the sensor that has the black line */
          frontLineSensorDischargeTimes[i - FRONT_LN_SEN_SENSOR_1_PIN] = BLACK;
          
          /* Flag that the line was found under the front sensor */
          lineUnderSensor.front = TRUE;
          
          /* Yes we found the line, but do not break, may as well check other sensors on this pass */
        }
        else {
          frontLineSensorDischargeTimes[i - FRONT_LN_SEN_SENSOR_1_PIN] = WHITE;
        }
      }
    }
  }

  /* If the line hasn't been detected under the left sensor, check the right sensor for the line */
  if (lineUnderSensor.left == FALSE) {
    /* Sample the Right Line Sensor */
    for (i = RIGHT_LN_SEN_SENSOR_1_PIN; i < RIGHT_LN_SEN_SENSOR_1_PIN + NUM_LINE_SENSOR_SENSORS; i++) {
      /* Check to see if the Line has already been found on this sensor */
      if (rightLineSensorDischargeTimes[i - RIGHT_LN_SEN_SENSOR_1_PIN] == BLACK) {
        /* The Line was found on this sensor, no need to check the whole sensor */
        lineUnderSensor.right = TRUE;
        
        /* Skip checking the rest of the sensor */
        break;
      }
      
      /* Check to see if the Pin is low. Mark the time it took to discharge
       *   the capacitor if the pin is low. */
      if (digitalRead(i) == LOW) {
        capDischargeTime = millis() - startCapChargeTime;
        if (capDischargeTime >= BLACK_LINE_MICROSEC_THRESHOLD_VALUE) {
          /* Flag the sensor that has the black line */
          rightLineSensorDischargeTimes[i - RIGHT_LN_SEN_SENSOR_1_PIN] = BLACK;
          
          /* Flag that the line was found under the front sensor */
          lineUnderSensor.right = TRUE;
          
          /* Yes we found the line, but do not break, may as well check other sensors on this pass */
        }
        else {
          rightLineSensorDischargeTimes[i - RIGHT_LN_SEN_SENSOR_1_PIN] = WHITE;
        }
      }
    }
  }

  /* If the line hasn't been detected under the right sensor, check the left sensor for the line */
  if (lineUnderSensor.right == FALSE) {
    /* Sample the Left Line Sensor */
    for (i = LEFT_LN_SEN_SENSOR_1_PIN; i < LEFT_LN_SEN_SENSOR_1_PIN + NUM_LINE_SENSOR_SENSORS; i++) {
      /* Check to see if the Line has already been found on this sensor */
      if (leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] == BLACK) {
        /* The line was found on this sensor, no need to check the whole sensor */
        lineUnderSensor.left = TRUE;
        
        /* Skip checking the rest of the sensor */
        break;
      }
      
      /* Check to see if the Pin is low. Mark the time it took to discharge
       *   the capacitor if the pin is low. */
      if (digitalRead(i) == LOW) {
        capDischargeTime = millis() - startCapChargeTime;
        if (capDischargeTime >= BLACK_LINE_MICROSEC_THRESHOLD_VALUE) {
          /* Flag the sensor that has the black line */
          leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] = BLACK;
          
          /* Flag that the line was found under the front sensor */
          lineUnderSensor.left = TRUE;
          
          /* Yes we found the line, but do not break, may as well check other sensors on this pass */
        }
        else {
          leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] = WHITE;
        }
      }
    }
  }
  
  /* If the line hasn't been detected under the right sensor, check the left sensor for the line */
  if (lineUnderSensor.right == FALSE) {
    /* Sample the Left Line Sensor */
    for (i = LEFT_LN_SEN_SENSOR_1_PIN; i < LEFT_LN_SEN_SENSOR_1_PIN + NUM_LINE_SENSOR_SENSORS; i++) {
      /* Check to see if the Line has already been found on this sensor */
      if (leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] == BLACK) {
        /* The line was found on this sensor, no need to check the whole sensor */
        lineUnderSensor.left = TRUE;
        
        /* Skip checking the rest of the sensor */
        break;
      }
      
      /* Check to see if the Pin is low. Mark the time it took to discharge
       *   the capacitor if the pin is low. */
      if (digitalRead(i) == LOW) {
        capDischargeTime = millis() - startCapChargeTime;
        if (capDischargeTime >= BLACK_LINE_MICROSEC_THRESHOLD_VALUE) {
          /* Flag the sensor that has the black line */
          leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] = BLACK;
          
          /* Flag that the line was found under the front sensor */
          lineUnderSensor.left = TRUE;
          
          /* Yes we found the line, but do not break, may as well check other sensors on this pass */
        }
        else {
          leftLineSensorDischargeTimes[i - LEFT_LN_SEN_SENSOR_1_PIN] = WHITE;
        }
      }
    }
  }
}


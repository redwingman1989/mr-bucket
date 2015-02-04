#include "lineSensor.h"

void lineSensorExec() {
  startCapChargeTime = 0; // Reset the cap charge times
  memset(frontLineSensorDischargeTimes, 0, NUM_LINE_SENSOR_SENSORS * sizeof(unsigned long)); // reset discharge time
  
  // Drive Sensor Line High
//  digitalWrite(FRONT_LN_SEN_SENSOR_1_PIN, HIGH);
//  digitalWrite(FRONT_LN_SEN_SENSOR_2_PIN, HIGH);
    // Direct Port Access
    PORTC = PORTC | B11111111;
  
  
  // Make the port an output
//  pinMode(FRONT_LN_SEN_SENSOR_1_PIN, OUTPUT);
//  pinMode(FRONT_LN_SEN_SENSOR_2_PIN, OUTPUT);
    // Direct Port Access
    DDRC = DDRC | B11111111;
    
  // Charge the lines for 10 us
  delayMicroseconds(10);
  
  // Make the port an input
//  pinMode(FRONT_LN_SEN_SENSOR_1_PIN, INPUT);
//  pinMode(FRONT_LN_SEN_SENSOR_2_PIN, INPUT);
    // Direct Port Access
    DDRC = DDRC & B00000000;
    
  // Mark the time of the start of charging the caps
  startCapChargeTime = micros();
  
  //delayMicroseconds(150);
  
//  pollLineSensor();
  
  // Start the Interrupt Timer that will poll the Line Sensor
  Timer3.start();
}







void pollLineSensor() {
  /* For each of the sensors on the line sensor, check to see if the capacitor has discharged. */
  for (unsigned char sensorNumOnLineSensor = 0; sensorNumOnLineSensor < NUM_LINE_SENSOR_SENSORS; sensorNumOnLineSensor++) {
    /* Line Sensor is on PORT C. Get the value of PORT  and mask each sensor slot to see if the sensor
     *   sees a white surface. After the Timer Interrupt triggers, the bits representing PORT C will be
     *   a value of 1 or 0.
     *   0 = White Surface 
     *   1 = Black/Dark Surface */
    if (((PINC & (1 << sensorNumOnLineSensor)) == FALSE)) {
      /* If the capacitor discharged, PORT C input channel will be 0. Mark the amount of time that it took. 
       *   If the time value is 0 after leaving this ISR, the Line Sensor's Sensor is over a dark or black
       *   surface. */
      frontLineSensorDischargeTimes[sensorNumOnLineSensor] = micros() - startCapChargeTime;
    } /* If (PINC...) */
  } /* For loop */
  
  if (DEBUG_BUILD == TRUE) {
    printOutLineSenData = TRUE;
    portA = PINC;
  } /* if(Debug) */
  
  //Timer3.stop();
}

#include <stdio.h>
#include <TimerOne.h>
#include <Wire.h>
#include "global.h"
#include "crc.h"
#include "irprox.h"
#include "atod.h"
#include "mag.h"
#include <math.h>

/* Preprocessor Debug indicator. change to 1 for debug build */
#define DEBUG 0

/*******************************************************************
 Function: void setup(void)
 Description: Arduino main function for initialization code.
*******************************************************************/
void setup() {
  delay(50); 
  crcInit();
  setupRobot(&Robot);
  setupTimerInt();
  setupSerial();
  setupPinModes();
  setupUltraSonicInt();
  setupMagnetometer();
  messageInit();
}

/********************************************************************
 Function: void loop(void)
 Description: Arduino main loop function. Runs continuously. Divided
  up into interrupt drven functions, including periodic, and interrupt
  dirven functions.
********************************************************************/
void loop() {
  loopStartTime = millis();
  /* Execute cycle() at 50Hz */
  if (iFlags.pit_50Hz == 1) {  
    /* execute main cycle */
    cycle();
    /* update icount */
    icount++;
    /* reset flag */
    iFlags.pit_50Hz = 0;      
  } 
  /* Execute readAD() at 500Hz */
  if (iFlags.pit_500Hz == 1) {
    /* Get Analog Radings */
    readAD(acount % NUM_ANALOGS);
    /* update acount */
    acount++;
    /* reset flag */
    iFlags.pit_500Hz = 0;
  }
  /* Excuted upon recording of right ultrasonic sensor echo */
  if (iFlags.ultraR == 1) {
    iFlags.ultraR = 0;
    updateUltrasonic(RIGHT);
    iFlags.sendR = 1;
  }
  /* Excuted upon recording of left ultrasonic sensor echo */
  if (iFlags.ultraL == 1) {
    iFlags.ultraL = 0;
    updateUltrasonic(LEFT);
    iFlags.sendL = 1;
  }
  loopTime = (millis() - loopStartTime);
  if (loopTime > maxLoopTime)
    maxLoopTime = loopTime;
}

/****************************************************************
 Function: void cycle(void)
 Description: Called from loop() at 50Hz. handles periodic tasks
 at 50Hz including:
   - Pulsing of Ultrasonics at alternating 25Hz
   - Blending Ultrasonic with IR Sensor
   - Formatting Sensor data into X coordinate
   - Determine Ball Color/Presence
   - Packetizing Data for transmission
   - Initiating packet transmission
****************************************************************/
void cycle() {
  
  /* Pulse Heartbeat LED on PIN 13 */
  heartbeat();  
  
  /* Alternating 25Hz Tasks */
  if (iFlags.pit_25Hz) {
    iFlags.pit_25Hz = 0;
    /* Trigger Left Ultrasonic */
    trigEcho(LEFT);
  }
  else {
    /* Trigger Right Ultrasonic */
    trigEcho(RIGHT);
  }
  
  /* Update Robot Members */
  updateRobot(&Robot);
  
  /* Packetize Data */
  packetizeMessage();
  
  /* Transmit Packet */
  transmitMessage();
  
}

/************************************************************ 
 Function: heartbeat()
 Description: I'm not explaining this.
************************************************************/
void heartbeat() {
  // Toggle Board LED at 2 Hz
  if (!(icount % 25))
    digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1); 
}

/****************************************************************
 Function: void PIT(void)
 Description: Function called at expiration of the periodic timer
 at 500Hz (every 2ms). This function sets the flags associated
 with periodic tasks to be executed form loop() and cycle().
*****************************************************************/
void PIT() {
  static unsigned int scaler = 0;

  iFlags.pit_500Hz = 1;
  
  if(++scaler % 10 == 0) {
    if (iFlags.pit_50Hz) {
      sprintf(outStr, "cycle overrun");
      Serial.println(outStr); 
    }
    iFlags.pit_50Hz = 1;
    if(scaler == 20) {
      iFlags.pit_25Hz = 1;
      scaler = 0; 
    }
  }
}

/****************************************************************
 Function: void updateRobot(robot)
 Description: 
****************************************************************/
void updateRobot(robot * bot) {  
  
  bot->heading = getHeading();
  
  if(bot->xCoordinate < 95)
    bot->zone = ZONE_0; //LEFT
  else if(bot->xCoordinate > 115)
    bot->zone = ZONE_2; //RIGHT
  else
    bot->zone = ZONE_1; //NEUTRAL
    
  /* Zone State Machine */
  switch(bot->zone) {
    /* ZONE_0 (0 < X <= 95), Listen to Left Sensors */
    case ZONE_0:
      bot->xCoordinate = bot->Ultra_X[LEFT];
      break;
    /* ZONE_1 (95 < X <= 115), Listen to Both Ultrasonics */
    case ZONE_1:
    
    if(bot->xCoordinate < 105) {
      bot->xCoordinate = bot->Ultra_X[LEFT];
    }
    else {
      bot->xCoordinate = bot->Ultra_X[RIGHT];
    }
//      /* If Ball Detected on Left, record X coordinate */
//      if (bot->Ultra_X[LEFT] + Course.stuckBall[LEFT] < 100) {
//        Course.stuckBall[LEFT] = bot->xCoordinate - bot->Ultra_X[LEFT];
//        bot->Ultra_X[LEFT] += Course.stuckBall[LEFT];
//      }
//      /* else, clear stuckball on left */
//      else
//        Course.stuckBall[LEFT] = 0;
//      /* Ball Detected on Right, record X coordinate */
//      if (bot->Ultra_X[RIGHT] - Course.stuckBall[RIGHT] > 110) {
//        Course.stuckBall[RIGHT] = 189 - bot->Ultra_X[RIGHT] + bot->xCoordinate;
//        bot->Ultra_X[RIGHT] -= (189 - Course.stuckBall[RIGHT]);
//      }
//      else
//        Course.stuckBall[RIGHT] = 0;
//        
//      if(bot->xCoordinate < 105) {
//        if (Course.stuckBall[LEFT] && !Course.stuckBall[RIGHT])
//          bot->xCoordinate = bot->Ultra_X[RIGHT];
//        else
//          bot->xCoordinate = bot->Ultra_X[LEFT];
//      }
//      else {
//        if (Course.stuckBall[RIGHT] && !Course.stuckBall[LEFT])
//          bot->xCoordinate = bot->Ultra_X[LEFT];
//        else
//          bot->xCoordinate = bot->Ultra_X[RIGHT];
//      }
      
      break;
    /* ZONE_2 (115 < X <= 189), Listen to Right Ultra Primary */
    case ZONE_2:
      bot->xCoordinate = bot->Ultra_X[RIGHT];
      break;
    default:
      break;
  } 
  
  /* Update Speed */
  
  /* Update Direction */
}





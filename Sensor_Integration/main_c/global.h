#ifndef _GLOBAL_H
#define _GLOBAL_H

// PIN Definitions
#define LED_PIN               (13)
#define IR_RIGHT_PIN          (A0)
#define IR_LEFT_PIN           (A1)
#define ANALOG_IN_SPARE       (A2)
#define RGB_RED_PIN           (A3)
#define RGB_GREEN_PIN         (A4)
#define RGB_BLUE_PIN          (A5)
#define ULTRA_ECHO_RPIN       (2)
#define ULTRA_ECHO_LPIN       (3)
#define ULTRA_TRIG_RPIN       (4)
#define ULTRA_TRIG_LPIN       (5)
#define ULTRA_RIGHT_ECHO_INT  (0)
#define ULTRA_LEFT_ECHO_INT   (1)

#define ULTRA_RIGHT_LEVER_ARM (6)
#define ULTRA_LEFT_LEVER_ARM  (6)
#define IR_RIGHT_LEVER_ARM    (4)
#define IR_LEFT_LEVER_ARM     (4)

#define Rx_BUFFER_SIZE (MESSAGE_LEN)
#define FILTER         (5)

/**********************************************************
 Interrupt Flag Structure and Declaration 
 This blows some peoples minds but all you need to know is 
 each unsigned char declatration :1 means its a 1 bit value
 of the intteruptFlagStruct. the extra struct wrap is to enable
 the ability to set the flags as a single byte. i.e. iFlags = 0;
***********************************************************/
struct interruptFlagStruct {
  union {
    struct {
      unsigned char pit_25Hz      :1;
      unsigned char pit_50Hz      :1;
      unsigned char pit_500Hz     :1;
      unsigned char uartRx    :1;
      unsigned char ultraR    :1;
      unsigned char ultraL    :1;
      unsigned char sendL     :1;
      unsigned char sendR     :1;
    };
    unsigned char allFlags;
  };
};
interruptFlagStruct iFlags;

enum message {
  SYNC_CHAR_1,                      // SYNC_CHAR_1 == 0xAA
  SYNC_CHAR_2,                      // SYNC_CAHR_2 == 0xAA
  SIZE_OF_MESSAGE_HEADER,
  DATA_0 = SIZE_OF_MESSAGE_HEADER,  // X - Coordinate
  DATA_1,                           // Ball Color/Presence
  DATA_2,                           // Front IR Sensor
  DATA_SIZE,
  CRC_IDX = DATA_SIZE,
  MESSAGE_LEN
};

enum side {
  RIGHT,        // RIGHT == 0
  LEFT,         // LEFT == 1
  NUM_SIDES     // NUM_SIDES == 2 (No shit?)
};

enum Zones {
  ZONE_0,   // 0 - 30 inches      (0 < X <= 60)
  ZONE_1,   // 30.5 - 64 inches   (60 < X <= 129)
  ZONE_2,   // 64.5 - 94.5 inches (129 < X <= 189)
};

struct robot {
  uint8_t Ultra_LeverArm[NUM_SIDES]; // Lever Arm Offset from Ultrasonics to CL
  uint8_t IR_LeverArm[NUM_SIDES];    // Lever Arm Offset from IRs to CL
  uint8_t Ultra_X[NUM_SIDES];  // X coordinate indicated by ultrasonics
  uint8_t IR_X[NUM_SIDES];     // X coordinate indicated by IRs
  uint8_t xCoordinate;   // xCoordinate blended to be sent
  uint8_t zone;          // current zone
  uint8_t dir;           // direction of movement
  uint8_t estSpeed;      // estinmated speed in divisions/second
  uint8_t ballCollor;    // ball color
}myRobot;

robot Robot;

uint8_t message[MESSAGE_LEN];

unsigned long UltraTime[NUM_SIDES];
int UltraDistance[NUM_SIDES];
unsigned long cycleStartTime;
unsigned long icount;

char outStr[100];
uint8_t serialBuf[Rx_BUFFER_SIZE];

#endif

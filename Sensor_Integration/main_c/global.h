#ifndef _GLOBAL_H
#define _GLOBAL_H

// PIN Definitions
#define LED_PIN               (13)
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
#define CYCLES_PER_SEC (50)

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
  DATA_1,                           // Left X-Coordinate
  DATA_2,                           // Right X-Coordinate
  DATA_3,
  DATA_4,
  DATA_5,
  DATA_6,
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
  ZONE_0,   // (0 < X <= 95)
  ZONE_1,   // (95 < X <= 115)
  ZONE_2,   // (115 < X <= 189)
};

struct robot {
  uint8_t Ultra_LeverArm[NUM_SIDES];    // Lever Arm Offset from Ultrasonics to CL
  uint8_t IR_LeverArm[NUM_SIDES];       // Lever Arm Offset from IRs to CL
  uint8_t Ultra_X[NUM_SIDES];           // X coordinate indicated by ultrasonics
  uint8_t IR_X[NUM_SIDES];              // X coordinate indicated by IRs
  uint8_t Blended_X[NUM_SIDES];         // X coordinate blended from same side sensors
  uint8_t xCoordinate;  // xCoordinate blended to be sent
  uint8_t zone;                         // current zone
  uint8_t dir;                          // direction of movement
  uint8_t estSpeed;                     // estinmated speed in divisions/second
  uint8_t ballCollor;                   // ball color
}myRobot;

robot Robot;

struct course {
  uint8_t stuckBall[NUM_SIDES];
};

course Course;

uint8_t message[MESSAGE_LEN];

enum colors {
  WHITE,
  BLUE,
  ORANGE,
  NUM_COLORS
};

struct ball {
  int redValue;
  int greenValue;
  int blueValue;
  uint8_t color;
};

ball loadedBall;

unsigned long UltraTime[NUM_SIDES];
int UltraDistance[NUM_SIDES];
unsigned long loopStartTime;
unsigned long icount;    // 50Hz/cycle counter
unsigned long acount;    // 500Hz/Analog counter

char outStr[100];
uint8_t serialBuf[Rx_BUFFER_SIZE];
uint8_t loopTime;
uint8_t maxLoopTime;

#endif

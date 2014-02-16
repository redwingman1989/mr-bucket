#ifndef _GLOBAL_H
#define _GLOBAL_H

// PIN Definitions
#define LED_PIN               (13)
#define IR_1_PIN              (A0)
#define ULTRA_ECHO_RPIN       (2)
#define ULTRA_ECHO_LPIN       (3)
#define ULTRA_TRIG_RPIN       (4)
#define ULTRA_TRIG_LPIN       (5)
#define ULTRA_RIGHT_ECHO_INT  (0)
#define ULTRA_LEFT_ECHO_INT   (1)

#define Rx_BUFFER_SIZE (MESSAGE_LEN)
#define FILTER         (5)

enum message {
  SYNC_CHAR_1,
  SYNC_CHAR_2,
  MESSAGE_ID, 
  DATA_LEN,
  SIZE_OF_MESSAGE_HEADER,
  DATA_0 = SIZE_OF_MESSAGE_HEADER,
  DATA_1,
  DATA_2,
  DATA_3,
  DATA_4,
  DATA_5,
  DATA_6,
  DATA_7,
  DATA_8,
  DATA_9,
  DATA_10,
  DATA_11,
  DATA_12,
  DATA_13,
  DATA_14,
  DATA_15,
  DATA_SIZE,
  CRC_IDX = DATA_SIZE,
  MESSAGE_LEN
};

enum side {
  RIGHT,
  LEFT,
  NUM_SIDES
};

/* Interrupt Flag Structure and Declaration */
struct interruptFlagStruct {
  union {
    struct {
      unsigned char pit       :1;
      unsigned char uartRx    :1;
      unsigned char ultraR    :1;
      unsigned char ultraL    :1;
      unsigned char sendL     :1;
      unsigned char sendR     :1;
      unsigned char reserved  :2;
    };
    unsigned char allFlags;
  };
};
interruptFlagStruct iFlags;

uint8_t message[NUM_SIDES][MESSAGE_LEN];

unsigned long UltraTime[NUM_SIDES];
int UltraDistance[NUM_SIDES];
unsigned long cycleStartTime;
unsigned long icount;

char outStr[100];
uint8_t serialBuf[Rx_BUFFER_SIZE];

#endif

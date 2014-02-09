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

#define MAX_LEN        (21)
#define Rx_BUFFER_SIZE (MAX_LEN)
#define FILTER         (5)

/* Interrupt Flag Structure and Declaration */
struct interruptFlagStruct {
  union {
    struct {
      unsigned char pit       :1;
      unsigned char uartRx    :1;
      unsigned char ultraR    :1;
      unsigned char ultraL    :1;
      unsigned char reserved  :4;
    };
    unsigned char allFlags;
  };
};
interruptFlagStruct iFlags;

unsigned long rUltraTime;
unsigned long lUltraTime;
double rUltraDistance;
unsigned long cycleStartTime;
unsigned long icount;

char outStr[100];
uint8_t serialBuf[Rx_BUFFER_SIZE];

enum message_ {
  SYNC_CHAR_1,
  SYNC_CHAR_2,
  MESSAGE_ID, 
  DATA_LEN,
  SIZE_OF_MESSAGE_HEADER,
  SIZE_MESSAGE_PADDING
};

#endif

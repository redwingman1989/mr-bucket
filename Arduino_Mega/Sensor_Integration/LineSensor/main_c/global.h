#ifndef _GLOBAL_H
#define _GLOBAL_H

/* PIN Definitions */
#define IR_1_PIN                    (A0)
#define ULTRA_RIGHT_ECHO_INT        (0)
#define ULTRA_LEFT_ECHO_INT         (1)
#define ULTRA_ECHO_RPIN             (2)
#define ULTRA_ECHO_LPIN             (3)
#define ULTRA_TRIG_RPIN             (4)
#define ULTRA_TRIG_LPIN             (5)
#define LED_PIN                     (13)
#define ALL_LN_SEN_LED_ENABLE_PIN   (14)
#define FRONT_LN_SEN_SENSOR_1_PIN   (22)
#define RIGHT_LN_SEN_SENSOR_1_PIN   (30)
#define REAR_LN_SEN_SENSOR_1_PIN    (28)
#define LEFT_LN_SEN_SENSOR_1_PIN    (46)
#define MAGNMTR_I2C_SDA_PIN         (20)
#define MAGNMTR_I2C_SCL_PIN         (21)

#define TRUE  (1)
#define FALSE (0)

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
      unsigned char pit_100Hz  :1;
      unsigned char spare0     :1;
      unsigned char spare1     :1;
      unsigned char spare2     :1;
      unsigned char spare3     :1;
      unsigned char spare4     :1;
      unsigned char spare5     :1;
      unsigned char spare6     :1;
    };
    unsigned char allFlags;
  };
};

/**************************** Global Variables *******************************/

interruptFlagStruct iFlags;

uint8_t message[MESSAGE_LEN];

unsigned long UltraTime[NUM_SIDES];
int UltraDistance[NUM_SIDES];

unsigned long loopStartTime;  // Variable Used for millis() at start of loop()
unsigned long icount;         // 50Hz/cycle counter
unsigned long acount;         // 500Hz/Analog counter

char outStr[100];
uint8_t serialBuf[Rx_BUFFER_SIZE];
uint8_t loopTime;
uint8_t maxLoopTime;

#endif

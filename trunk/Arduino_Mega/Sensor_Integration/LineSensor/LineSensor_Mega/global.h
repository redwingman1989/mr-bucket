#ifndef _GLOBAL_H
#define _GLOBAL_H

/* Preprocessor Debug indicator. Change to 1 for debug build */
#define DEBUG_BUILD                 (1)

/* Setup Macro Values (used in Setup loop) */
// Serial Port
#define SERIAL_BAUD_RATE            (115200)
// Programmable Interrupt Timer
#define PIT_PERIOD_IN_MICROSECS     (20000)
#define LINE_SENSOR_POLL_PERIOD     (200) // This should be a calibrated value!



/* PIN Definitions */
// Board LED
#define LED_PIN                     (13)
// Line Sensor
#define ALL_LN_SEN_LED_ENABLE_PIN   (7)
#define FRONT_LN_SEN_SENSOR_1_PIN   (37) // Port C - 0
#define FRONT_LN_SEN_SENSOR_2_PIN   (36) // Port C - 1
#define FRONT_LN_SEN_SENSOR_3_PIN   (35) // Port C - 2
#define FRONT_LN_SEN_SENSOR_4_PIN   (34) // Port C - 3
#define FRONT_LN_SEN_SENSOR_5_PIN   (33) // Port C - 4
#define FRONT_LN_SEN_SENSOR_6_PIN   (32) // Port C - 5
#define FRONT_LN_SEN_SENSOR_7_PIN   (31) // Port C - 6
#define FRONT_LN_SEN_SENSOR_8_PIN   (30) // Port C - 7
#define NUM_LINE_SENSOR_SENSORS     (8)

/* Boolean Values */
#define TRUE                        (1)
#define FALSE                       (0)

/* Interrupt Flag Structure and Declaration */
struct interruptFlagStruct {
  union {
    struct {
      unsigned char pit_50Hz  :1;
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

unsigned long loopStartTime;  // Variable Used for millis() at start of loop()
unsigned long icount;         // 50Hz/cycle counter
unsigned long acount;         // 500Hz/Analog counter

unsigned char printOutLineSenData;


uint8_t loopTime;
uint8_t maxLoopTime;

#endif

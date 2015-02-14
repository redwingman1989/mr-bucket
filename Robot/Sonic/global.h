#ifndef _GLOBAL_H
#define _GLOBAL_H

/* Preprocessor Debug indicator. Change to 1 for debug build */
#define DEBUG_BUILD                 (1)

// Programmable Interrupt Timer
#define PIT_PERIOD_IN_MICROSECS     (20000)
#define LINE_SENSOR_POLL_PERIOD     (200) // This should be a calibrated value!

/* Setup Macro Values (used in Setup loop) */
// Serial Port
#define SERIAL_BAUD_RATE            (115200)
/* PIN Definitions */
// Board LED
#define LED_PIN                     (13)



/**************************** Global Variables *******************************/

static unsigned long loopStartTime;  // Variable Used for millis() at start of loop()
static unsigned long icount;         // 50Hz/cycle counter
static unsigned long acount;         // 500Hz/Analog counter

static unsigned char printOutLineSenData;


static unsigned char loopTime;
static unsigned char maxLoopTime;

#endif

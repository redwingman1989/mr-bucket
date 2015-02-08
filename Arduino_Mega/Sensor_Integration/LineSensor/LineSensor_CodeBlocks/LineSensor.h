#ifndef _LINESENSOR_H
#define _LINESENSOR_H

/* Macros */

/* PORT A */
#define PORTA_PIN_0 (22)
#define PORTA_PIN_1 (23)
#define PORTA_PIN_2 (24)
#define PORTA_PIN_3 (25)
#define PORTA_PIN_4 (26)
#define PORTA_PIN_5 (27)
#define PORTA_PIN_6 (28)
#define PORTA_PIN_7 (29)

/* PORT B */
#define PORTB_PIN_0 (53)
#define PORTB_PIN_1 (52)
#define PORTB_PIN_2 (51)
#define PORTB_PIN_3 (50)
#define PORTB_PIN_4 (10)
#define PORTB_PIN_5 (11)
#define PORTB_PIN_6 (12)
#define PORTB_PIN_7 (13)

/* PORT C */
#define PORTC_PIN_0 (37)
#define PORTC_PIN_1 (36)
#define PORTC_PIN_2 (35)
#define PORTC_PIN_3 (34)
#define PORTC_PIN_4 (33)
#define PORTC_PIN_5 (32)
#define PORTC_PIN_6 (31)
#define PORTC_PIN_7 (30)

/* PORT L */
#define PORTL_PIN_0 (49)
#define PORTL_PIN_1 (48)
#define PORTL_PIN_2 (47)
#define PORTL_PIN_3 (46)
#define PORTL_PIN_4 (45)
#define PORTL_PIN_5 (44)
#define PORTL_PIN_6 (43)
#define PORTL_PIN_7 (42)

/* Enumerations */
enum lineSensorSensors {
  SENSOR_1,
  SENSOR_2,
  SENSOR_3,
  SENSOR_4,
  SENSOR_5,
  SENSOR_6,
  SENSOR_7,
  SENSOR_8,
  NUM_LINE_SENSOR_SENSORS
};

/* Center Front Line Sensor is on PORT A */
static unsigned char centerFront[NUM_LINE_SENSOR_SENSORS] = {PORTA_PIN_0, PORTA_PIN_1, PORTA_PIN_2,
                                                             PORTA_PIN_3, PORTA_PIN_4, PORTA_PIN_5,
                                                             PORTA_PIN_6, PORTA_PIN_7};
/* Center Back Line Sensor is on PORT B */
static unsigned char centerBack[NUM_LINE_SENSOR_SENSORS]  = {PORTB_PIN_0, PORTB_PIN_1, PORTB_PIN_2,
                                                             PORTB_PIN_3, PORTB_PIN_4, PORTB_PIN_5,
                                                             PORTB_PIN_6, PORTB_PIN_7};

/* Side Front Line Sensor is on PORT C */
static unsigned char sideFront[NUM_LINE_SENSOR_SENSORS]   = {PORTC_PIN_0, PORTC_PIN_1, PORTC_PIN_2,
                                                             PORTC_PIN_3, PORTC_PIN_4, PORTC_PIN_5,
                                                             PORTC_PIN_6, PORTC_PIN_7};

/* Side Back Line Sensor is on PORT L */
static unsigned char sideBack[NUM_LINE_SENSOR_SENSORS]    = {PORTL_PIN_0, PORTL_PIN_1, PORTL_PIN_2,
                                                             PORTL_PIN_3, PORTL_PIN_4, PORTL_PIN_5,
                                                             PORTL_PIN_6, PORTL_PIN_7};

typedef union readings_t {
     struct {
       unsigned char sensor1 : 1;
       unsigned char sensor2 : 1;
       unsigned char sensor3 : 1;
       unsigned char sensor4 : 1;
       unsigned char sensor5 : 1;
       unsigned char sensor6 : 1;
       unsigned char sensor7 : 1;
       unsigned char sensor8 : 1;
     } individualReadings;
     unsigned char allReadings;
 } readings;

class LineSensor {
  public:
    //Constructor
    LineSensor(unsigned char * inPinMap);

    // Methods
    void beginCheck();
    void getReading();

  private:
    // Pin Numbers
    unsigned char * pinMap;
//    unsigned char pinSen2;
//    unsigned char pinSen3;
//    unsigned char pinSen4;
//    unsigned char pinSen5;
//    unsigned char pinSen6;
//    unsigned char pinSen7;
//    unsigned char pinSen8;
    // Readings from the Line Sensor
    readings sensorReadings;
    // Start Cap Charge Time;
    unsigned long startCapChargeTime;

};


#endif

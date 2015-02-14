#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#include <stdint.h>

/* Macros */

/* PORT A */
#define PORTA_DATA_REG     (0x22)
#define PORTA_DATA_DIR_REG (0x21)
#define PORTA_IN_PINS_REG  (0x20)
  /* Arduino Pins */
#define PORTA_PIN_0 (22)
#define PORTA_PIN_1 (23)
#define PORTA_PIN_2 (24)
#define PORTA_PIN_3 (25)
#define PORTA_PIN_4 (26)
#define PORTA_PIN_5 (27)
#define PORTA_PIN_6 (28)
#define PORTA_PIN_7 (29)

/* PORT B */
#define PORTB_DATA_REG     (0x25)
#define PORTB_DATA_DIR_REG (0x24)
#define PORTB_IN_PINS_REG  (0x23)
  /* Arduino Pins */
#define PORTB_PIN_0 (53)
#define PORTB_PIN_1 (52)
#define PORTB_PIN_2 (51)
#define PORTB_PIN_3 (50)
#define PORTB_PIN_4 (10)
#define PORTB_PIN_5 (11)
#define PORTB_PIN_6 (12)
#define PORTB_PIN_7 (13)

/* PORT C */
#define PORTC_DATA_REG     (0x28)
#define PORTC_DATA_DIR_REG (0x27)
#define PORTC_IN_PINS_REG  (0x26)
  /* Arduino Pins */
#define PORTC_PIN_0 (37)
#define PORTC_PIN_1 (36)
#define PORTC_PIN_2 (35)
#define PORTC_PIN_3 (34)
#define PORTC_PIN_4 (33)
#define PORTC_PIN_5 (32)
#define PORTC_PIN_6 (31)
#define PORTC_PIN_7 (30)

/* PORT L */
#define PORTL_DATA_REG     (0x10B)
#define PORTL_DATA_DIR_REG (0x10A)
#define PORTL_IN_PINS_REG  (0x109)
  /* Arduino Pins */
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
static const uint8_t centerFront[NUM_LINE_SENSOR_SENSORS] = {PORTA_PIN_0, PORTA_PIN_1, PORTA_PIN_2,
                                                             PORTA_PIN_3, PORTA_PIN_4, PORTA_PIN_5,
                                                             PORTA_PIN_6, PORTA_PIN_7};
/* Center Back Line Sensor is on PORT B */
static const uint8_t centerBack[NUM_LINE_SENSOR_SENSORS]  = {PORTB_PIN_0, PORTB_PIN_1, PORTB_PIN_2,
                                                             PORTB_PIN_3, PORTB_PIN_4, PORTB_PIN_5,
                                                             PORTB_PIN_6, PORTB_PIN_7};

/* Side Front Line Sensor is on PORT C */
static const uint8_t sideFront[NUM_LINE_SENSOR_SENSORS]   = {PORTC_PIN_0, PORTC_PIN_1, PORTC_PIN_2,
                                                             PORTC_PIN_3, PORTC_PIN_4, PORTC_PIN_5,
                                                             PORTC_PIN_6, PORTC_PIN_7};

/* Side Back Line Sensor is on PORT L */
static const uint8_t sideBack[NUM_LINE_SENSOR_SENSORS]    = {PORTL_PIN_0, PORTL_PIN_1, PORTL_PIN_2,
                                                             PORTL_PIN_3, PORTL_PIN_4, PORTL_PIN_5,
                                                             PORTL_PIN_6, PORTL_PIN_7};

class LineSensor {
  public:
    typedef union readings_t
    {
      struct {
        uint8_t sensor1 : 1;
        uint8_t sensor2 : 1;
        uint8_t sensor3 : 1;
        uint8_t sensor4 : 1;
        uint8_t sensor5 : 1;
        uint8_t sensor6 : 1;
        uint8_t sensor7 : 1;
        uint8_t sensor8 : 1;
      } individualReadings;
    uint8_t allReadings;
    } readings;


    //Constructor
    LineSensor(const uint8_t * inPinMap);
    void beginCheck1();
    void beginCheck2();
    void takeReading();

    // Methods
    uint8_t getLineSensorReadings();

  private:
    /* Methods */

    /* Sensor Pin Numbers */
    const uint8_t * pinMap;

    /* Readings from the Line Sensor */
    readings sensorReadings;

    /* Start Cap Charge Time in us */
    uint32_t startCapChargeTime;

    /* Pointer to Sensor's Data Register */
    uint8_t * ptrPortDataReg;

    /* Pointer to Sensor's Data Direction Register */
    uint8_t * ptrPortDataDirReg;

    /* Pointer to Sensor's PORT Input Pins */
    uint8_t * ptrPortInputPins;

};


#endif

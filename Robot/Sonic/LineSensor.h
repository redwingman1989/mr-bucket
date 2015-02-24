#pragma once

#include "System/RunableModule.h"
#include "Pins.h"
/* Macros */

/* PORT A */
#define PORTA_DATA_REG     (0x22)
#define PORTA_DATA_DIR_REG (0x21)
#define PORTA_IN_PINS_REG  (0x20)

/* PORT B */
#define PORTB_DATA_REG     (0x25)
#define PORTB_DATA_DIR_REG (0x24)
#define PORTB_IN_PINS_REG  (0x23)

/* PORT C */
#define PORTC_DATA_REG     (0x28)
#define PORTC_DATA_DIR_REG (0x27)
#define PORTC_IN_PINS_REG  (0x26)

/* PORT L */
#define PORTL_DATA_REG     (0x10B)
#define PORTL_DATA_DIR_REG (0x10A)
#define PORTL_IN_PINS_REG  (0x109)


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
static const uint8_t centerFront[NUM_LINE_SENSOR_SENSORS] =
  {pinLineCF_0, pinLineCF_1, pinLineCF_2, pinLineCF_3,
   pinLineCF_4, pinLineCF_5, pinLineCF_6, pinLineCF_7};

/* Center Back Line Sensor is on PORT B */
static const uint8_t centerBack[NUM_LINE_SENSOR_SENSORS] =
  {pinLineCB_0, pinLineCB_1, pinLineCB_2, pinLineCB_3,
   pinLineCB_4, pinLineCB_5, pinLineCB_6, pinLineCB_7};

/* Side Front Line Sensor is on PORT C */
static const uint8_t sideFront[NUM_LINE_SENSOR_SENSORS] =
  {pinLineRF_0, pinLineRF_1, pinLineRF_2, pinLineRF_3,
   pinLineRF_4, pinLineRF_5, pinLineRF_6, pinLineRF_7};

/* Side Back Line Sensor is on PORT L */
static const uint8_t sideBack[NUM_LINE_SENSOR_SENSORS] =
  {pinLineRB_0, pinLineRB_1, pinLineRB_2, pinLineRB_3,
   pinLineRB_4, pinLineRB_5, pinLineRB_6, pinLineRB_7};

class LineSensor{
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
    ~LineSensor();

    void startCharging();
    void stopCharging();
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


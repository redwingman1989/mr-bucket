#ifndef _ULTRASONICSENSOR_H
#define _ULTRASONICSENSOR_H

//#include "System\RunableModule.h"

/* Macros */

/* Front Facing UltraSonic */
#define ULTRA_FRONT_TRIG_PIN          (7)
#define ULTRA_FRONT_ECHO_PIN          (18) /* PD3 = INT3 */

/* Left Facing UltraSonic */
#define ULTRA_LEFT_TRIG_PIN          (8)
#define ULTRA_LEFT_ECHO_PIN          (19) /* PD2 = INT2 */

/* Right Facing UltraSonic */
#define ULTRA_RIGHT_TRIG_PIN          (9)
#define ULTRA_RIGHT_ECHO_PIN          (2) /* PE4 = INT4 */

/* Enumerations */
enum ultraSonicPins {
  TRIGGER_PIN,
  ECHO_PIN,
  NUM_ULTRA_PINS
};

enum readings {
  READING_1,
  READING_2,
  READING_3,
  READING_4,
  READING_5,
  NUM_ULTRA_FILT_READINGS
};

enum sensors {
  FRONT,
  LEFT,
  RIGHT,
  NUM_ULTRA_SENSORS
};

/* Front Facing UltraSonic Sensor Pin Mappings */
static const uint8_t frontUltraSonic[NUM_ULTRA_PINS] = {ULTRA_FRONT_TRIG_PIN, ULTRA_FRONT_ECHO_PIN};

/* Left Facing UltraSonic Sensor Pin Mappings */
static const uint8_t leftUltraSonic[NUM_ULTRA_PINS] = {ULTRA_LEFT_TRIG_PIN, ULTRA_LEFT_ECHO_PIN};

/* Right Facing UltraSonic Sensor Pin Mappings */
static const uint8_t rightUltraSonic[NUM_ULTRA_PINS] = {ULTRA_RIGHT_TRIG_PIN, ULTRA_RIGHT_ECHO_PIN};


class UltraSonicSensor {
  public:
    /* Constructor */
    UltraSonicSensor(const uint8_t * inPinMap);

    /* Deconstructor */
    ~UltraSonicSensor();

//    /* Initialize Echo Interrupt */
//    void initEchoInterrupt();

//    /* Configure External Interrupt Control Register */
//    void configureExtIntCtrlRegister(uint8_t * ptrRegister);

    /* Set the echo pulse start time */
    void setFirstEchoTime(uint32_t timeInMicroseconds);

    /* Set the echo pulse end time */
    void setLastEchoTime(uint32_t timeInMicroseconds);

    /* Set the duration the Trigger pin was high */
    void setReadyForDistanceCalc(bool timeForDistanceCalc);

    /* Set a flag indicating whether or not a reading is in progress */
    void setReadingInProgress(bool inProgress);

    /* Set a flag indicating the sensor is invalid */
    void setInvalidSensorFlag(bool invalid);

    /* Get the echo pulse start time */
    uint32_t getFirstEchoTime();

    /* Get the duration of time the Trigger Pin was high */
    uint32_t getTrigPinHighDuration();

    /* Get the Trigger Pin */
    uint8_t getTriggerPin();

    /* Get the Echo Pin */
    uint8_t getEchoPin();

    /* Get a look at the flag that indicates that it is time for a calculation */
    bool getTimeForCalcFlag();

    /* Get whether or not a reading is in progress */
    bool getReadingInProgress();

    /* Get whether or not the sensor has been marked as invalid */
    bool getInvalidStatus();

//    /* The ISR called when the Echo pin interrupts the Mega */
//    void echoISR();

    /* Set the trigger pin */
    void triggerAPulse(void);

    /* Calculate the distance to reflected surface */
    uint8_t calculateDistance();

    /* Implement functions from the parent class */
//    bool RunTick(uint16_t time, RobotState state);
//    void DebugOutput(HardwareSerial * hwSerialPort);

  private:
    /* Sensor Pin Numbers */
    const uint8_t * pinMap;

    /* Raw Data Array */
    uint8_t rawDataArray[NUM_ULTRA_FILT_READINGS];

    /* Current Raw Data Array Index */
    uint8_t rawDataArrayIdx;

    /* Trigger Pin set high at this time in us */
    uint32_t triggerStartTime;

    /* Time duration the trigger pin was set high in us*/
    uint32_t trigPinHighDuration;

//    /* External Interrupt Number for this UltraSonic Sensor */
//    uint8_t extInterruptNumber;

    /* Echo 1st Pulse Receive time */
    volatile uint32_t rxFirstEchoTime;

    /* Echo last pulse Receive time */
    volatile uint32_t rxLastEchoTime;

    /* Flag indicating that it is time to run calculations on ISR timing data */
    bool timeToCalculateDistance;

    /* Boolean indicating that a reading is in progress */
    bool readingInProgress;

    /* Flag indicating that the sensor is invalid due to some issue */
    bool invalidFlag;

};

     /*************************************************************
 * Function:     calculateDistance
 * Input:        void
 * Return:       void
 * Description:  This function calculates the disatance to the
 *                 reflected object/surface, based on the duration
 *                 of time the Trigger pin was set high.
 *           According to the datasheet:
 *             range = high level time * velocity of Ultrasonic Wave / 2
 *
 *           - Velocity of the UltraSonic Wave = ~ 340 meters/second
 *                                             = ~ 13,386 inches/second
 *                                             = ~ 0.013386 inches/microsecond
 *************************************************************/
inline uint8_t UltraSonicSensor::calculateDistance()
{
    #define VELOCITY_ULTRA_WAVE_IN_MICROSECONDS (0.013386)
    uint32_t timeDelta = this->rxLastEchoTime - this->rxFirstEchoTime;
    float timeDeltaFloat = (float) timeDelta;
    float finalVal = (uint8_t)((timeDeltaFloat) * VELOCITY_ULTRA_WAVE_IN_MICROSECONDS / 2.0);

    Serial.print("calcDist delta float: ");
    Serial.println(finalVal);

    return (uint8_t) finalVal;

    #undef VELOCITY_ULTRA_WAVE_IN_MICROSECONDS
}

#endif // _ULTRASONICSENSOR_H

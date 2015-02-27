#ifndef _ULTRASONICSENSOR_H
#define _ULTRASONICSENSOR_H

#include "Arduino.h"

/* Enumerations */

enum readings {
  READING_1,
  READING_2,
  READING_3,
  READING_4,
  READING_5,
  NUM_ULTRA_FILT_READINGS
};

typedef enum {
  FRONT,
  LEFT,
  RIGHT,
  NUM_ULTRA_SENSORS
} ultSensor_t;


class UltraSonicSensor {
  public:
    /* Constructors */
    UltraSonicSensor();
    UltraSonicSensor(uint8_t trigPin, uint8_t echPin);

    /* Deconstructor */
    ~UltraSonicSensor();


    /* SETTERS */

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


    /* GETTERS */

    /* Get the last echo pulse rx time */
    uint32_t getLastEchoTime();

    /* Get the echo pulse start time */
    uint32_t getFirstEchoTime();

    /* Get the Trigger Pin */
    uint8_t getTriggerPin();

    /* Get the Echo Pin */
    uint8_t getEchoPin();

    /* Get a look at the flag that indicates that it is time for a calculation */
    bool getTimeForCalcFlag();

    /* Get whether or not a reading is in progress */
    bool getReadingInProgress();

    /* Get whether or not the sensor has been marked as invalid */
    bool sensorInvalid();

    /* Get the last calculated distance */
    float getCalculatedDistanceValue();


    /* OTHER FUNCTIONALITY */

    /* Trigger a pulse */
    void triggerAPulse(void);

    /* Calculate the distance to reflected surface */
    float calculateDistance();

  private:
    /* Trigger Pin */
    uint8_t triggerPin;

    /* Echo Pin */
    uint8_t echoPin;

    /* Raw Data Array */
    uint8_t rawDataArray[NUM_ULTRA_FILT_READINGS];

    /* Current Raw Data Array Index */
    uint8_t rawDataArrayIdx;

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

    /* Calculated Distance */
    float calcDist;

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
inline float UltraSonicSensor::calculateDistance()
{
    #define VELOCITY_ULTRA_WAVE_IN_MICROSECONDS (0.013386)
    uint32_t timeDelta = this->rxLastEchoTime - this->rxFirstEchoTime;
    float timeDeltaFloat = (float) timeDelta;
    float finalVal = ((timeDeltaFloat) * VELOCITY_ULTRA_WAVE_IN_MICROSECONDS * 0.5);

    calcDist = ((timeDeltaFloat) * VELOCITY_ULTRA_WAVE_IN_MICROSECONDS * 0.5);

    //return finalVal;

    #undef VELOCITY_ULTRA_WAVE_IN_MICROSECONDS
}

#endif // _ULTRASONICSENSOR_H

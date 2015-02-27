#include <Arduino.h>
#include "Pins.h"
#include "Interrupts.h"
#include "UltraSonicISR.h"
#include "UltraSonicSensor.h"

 /*************************************************************
 * Function:     UltraSonicSensor Constructor
 * Input:        uint8_t * inPinMap
 * Return:       void
 * Description:  This is the constructor for an UltraSonicSensor
 *                 object. The constructor takes a pointer to an
 *                 array that contains the Trigger pin and the
 *                 Echo pin values for the UltraSonicSensor object.
 *************************************************************/
 UltraSonicSensor::UltraSonicSensor():
                                    triggerPin(0),
                                    echoPin(0),
                                    rawDataArrayIdx(0),
                                    rxFirstEchoTime(0),
                                    rxLastEchoTime(0),
                                    timeToCalculateDistance(false),
                                    invalidFlag(false),
                                    readingInProgress(false)

{
    /* Initialize the Raw Data Array */
    memset(this->rawDataArray, 0, sizeof(uint8_t) * NUM_ULTRA_FILT_READINGS);
}


 /*************************************************************
 * Function:     UltraSonicSensor Constructor
 * Input:        uint8_t * inPinMap
 * Return:       void
 * Description:  This is the constructor for an UltraSonicSensor
 *                 object. The constructor takes a pointer to an
 *                 array that contains the Trigger pin and the
 *                 Echo pin values for the UltraSonicSensor object.
 *************************************************************/
 UltraSonicSensor::UltraSonicSensor(uint8_t trigPin, uint8_t echoPin):
                                    triggerPin(trigPin),
                                    echoPin(echoPin),
                                    rawDataArrayIdx(0),
                                    rxFirstEchoTime(0),
                                    rxLastEchoTime(0),
                                    timeToCalculateDistance(false),
                                    invalidFlag(false),
                                    readingInProgress(false)

{
    /* Initialize the Raw Data Array */
    memset(this->rawDataArray, 0, sizeof(uint8_t) * NUM_ULTRA_FILT_READINGS);

    /* Set the Triger Pin to an output */
    pinMode(triggerPin, OUTPUT);

    /* Set the Echo Pin to an input */
    pinMode(echoPin, INPUT);

    /* Attach the Interrupts to the Echo Pins */
    if (echoPin == pinFUltraEcho)
        attachInterrupt((uint8_t)ULTRA_FRONT_EXT_INTERRUPT_NUM, frontUltraSonicISR, CHANGE);
    else if (echoPin == pinLUltraEcho)
      attachInterrupt((uint8_t)ULTRA_LEFT_EXT_INTERRUPT_NUM, leftUltraSonicISR, CHANGE);
    else if (echoPin == pinRUltraEcho)
      attachInterrupt((uint8_t)ULTRA_RIGHT_EXT_INTERRUPT_NUM, rightUltraSonicISR, CHANGE);
    else
      Serial.println("Error Initializing UltraSonic Interrupt!");
}


 /*************************************************************
 * Function:     UltraSonicSensor Destructor
 * Input:        void
 * Return:       void
 * Description:  Destructor for an UltraSonicSensor object
 *************************************************************/
 UltraSonicSensor::~UltraSonicSensor()
 {
     free(this);
 }


 /*************************************************************
 * Function:     setFirstEchoTime()
 * Parameter:    uint32_t timeInMicroseconds
 * Return:       void
 * Description:  Set the class member variable for the time in
 *                 microseconds of the first received echo pulse.
 *************************************************************/
void UltraSonicSensor::setFirstEchoTime(uint32_t timeInMicroseconds)
{
    rxFirstEchoTime = timeInMicroseconds;
}


 /*************************************************************
 * Function:     setLastEchoTime()
 * Parameter:    uint32_t timeInMicroseconds
 * Return:       void
 * Description:  Set the class member variable for the time in
 *                 microseconds of the last received echo pulse.
 *************************************************************/
void UltraSonicSensor::setLastEchoTime(uint32_t timeInMicroseconds)
{
    rxLastEchoTime = timeInMicroseconds;
}


 /*************************************************************
 * Function:     setReadyForDistanceCalc()
 * Parameter:    bool timeForDistanceCalc
 * Return:       void
 * Description:  Set the class member variable that wil be used to
 *                 see if it is time to calculate distances.
 *************************************************************/
void UltraSonicSensor::setReadyForDistanceCalc(bool timeForDistanceCalc)
{
    timeToCalculateDistance = timeForDistanceCalc;
}


 /*************************************************************
 * Function:     setReadingInProgress()
 * Parameter:    bool inProgress
 * Return:       void
 * Description:  Set the class member variable that wil be used to
 *                 see if a reading is in progress.
 *************************************************************/
void UltraSonicSensor::setReadingInProgress(bool inProgress)
{
    readingInProgress = inProgress;
}


 /*************************************************************
 * Function:     setInvalidSensorFlag()
 * Parameter:    bool inProgress
 * Return:       void
 * Description:  Set the class member variable that wil be used to
 *                 see if a reading is in progress.
 *************************************************************/
void UltraSonicSensor::setInvalidSensorFlag(bool invalid)
{
    invalidFlag = invalid;
}


 /*************************************************************
 * Function:     getLastEchoTime()
 * Parameter:    void
 * Return:       uint32_t timeInMicroseconds
 * Description:  Get the class member variable for the time in
 *                 microseconds of the last received echo pulse.
 *************************************************************/
uint32_t UltraSonicSensor::getLastEchoTime(void)
{
    return this->rxLastEchoTime;
}


 /*************************************************************
 * Function:     getFirstEchoTime
 * Input:        void
 * Return:       uint8_t echoPin
 * Description:  Returns the time of the first echo pulse.
 *************************************************************/
uint32_t UltraSonicSensor::getFirstEchoTime()
{
    return (this->rxFirstEchoTime);
}


 /*************************************************************
 * Function:     getTriggerPin
 * Input:        void
 * Return:       uint8_t triggerPin
 * Description:  Returns the value of the UltraSonicSensor
 *                 object's Trigger pin.
 *************************************************************/
uint8_t UltraSonicSensor::getTriggerPin()
{
    return (this->triggerPin);
}


 /*************************************************************
 * Function:     getEchoPin
 * Input:        void
 * Return:       uint8_t echoPin
 * Description:  Returns the value of the UltraSonicSensor
 *                 object's Echo pin.
 *************************************************************/
uint8_t UltraSonicSensor::getEchoPin()
{
    return (this->echoPin);
}


 /*************************************************************
 * Function:     getTimeForCalcFlag
 * Input:        void
 * Return:       bool timeToCalculateDistance
 * Description:  Returns whether or not it is time to run distance
 *                 calculations.
 *************************************************************/
bool UltraSonicSensor::getTimeForCalcFlag()
{
    return (this->timeToCalculateDistance);
}


 /*************************************************************
 * Function:     getReadingInProgress
 * Input:        void
 * Return:       bool readingInProgress
 * Description:  Returns whether or not a reading is in progress.
 *                 This is used tpo prevent multiple UltraSonic
 *                 sensors from sending out pulses at the same time.
 *************************************************************/
bool UltraSonicSensor::getReadingInProgress()
{
    return (this->readingInProgress);
}


 /*************************************************************
 * Function:     sensorInvalid
 * Input:        void
 * Return:       bool
 * Description:  Returns sensor validity status.
 *************************************************************/
bool UltraSonicSensor::sensorInvalid()
{
    return (this->invalidFlag);
}


 /*************************************************************
 * Function:     triggerAPulse()
 * Parameter:    void
 * Return:       void
 * Description:  Sets the trigger pin high for 10 microseconds.
 *                 This triggers an ultrasonic wave to be produced
 *                 by the ultrasonic sensor.
 *************************************************************/
void UltraSonicSensor::triggerAPulse()
{
  this->readingInProgress = true;
  digitalWrite(this->triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->triggerPin, LOW);
}


// /*************************************************************
// * Function:     calculateDistance
// * Input:        void
// * Return:       void
// * Description:  This function calculates the disatance to the
// *                 reflected object/surface, based on the duration
// *                 of time the Trigger pin was set high.
// *           According to the datasheet:
// *             range = high level time * velocity of Ultrasonic Wave / 2
// *
// *           - Velocity of the UltraSonic Wave = ~ 340 meters/second
// *                                             = ~ 13,386 inches/second
// *                                             = ~ 0.013386 inches/microsecond
// *************************************************************/
//inline uint8_t UltraSonicSensor::calculateDistance()
//{
//    #define VELOCITY_ULTRA_WAVE_IN_MICROSECONDS (0.013386)
//
//    return (uint8_t)((float)(this->rxLastEchoTime - this->rxFirstEchoTime)) *
//                      (float) VELOCITY_ULTRA_WAVE_IN_MICROSECONDS * 0.5;
//
//    #undef VELOCITY_ULTRA_WAVE_IN_MICROSECONDS
//}

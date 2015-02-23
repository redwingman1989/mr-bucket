#include "UltraSonicSensor.h"
#include "UltraSonicManager.h"



 /*************************************************************
 * Function:     UltraSonicManager Constructor
 * Input:        UltraSonicSensor ** ptrUltraSonicObjArray
 * Return:       void
 * Description:  This is the constructor for an UltraSonicManager
 *                 object. The constructor takes a pointer to an
 *                 array of pointers to UltraSonicSensor objects.
 *************************************************************/
UltraSonicManager::UltraSonicManager(UltraSonicSensor ** ptrUltraSonicObjArray) :
                                     ptrToUltraSonicObjArray(ptrUltraSonicObjArray),
                                     sensorToExec(FRONT)
{

}


 /*************************************************************
 * Function:     UltraSonicManager Destructor
 * Input:        void
 * Return:       void
 * Description:  Destructor for an UltraSonicManager object
 *************************************************************/
UltraSonicManager::~UltraSonicManager()
{
  free(this);
}


 /*************************************************************
 * Function:     RunTick()
 * Parameter:    void
 * Return:       void
 * Description:  This is a virtual function inherited from the parent class:
 *                 RunableModule.
 *************************************************************/
bool UltraSonicManager::RunTick(uint16_t time, RobotState state)
{
  #define ECHO_TIMEOUT (10000)

  /* previousSensorExec is initialized to the next sensor to exec, it
   *   must be updated before it is used!!!! This way we can initialize
   *   it to a value that is possible to be held in a uint8_t */
  uint8_t previousSensorExec = this->sensorToExec;
  uint32_t currentMicrosTime = micros();

  /* By knowing what the next sensor to exec is, we need to look at the sensor
   *   prior and see if it is in the middle of taking a reading. Handle wrap around. */
  if (this->sensorToExec == 0)
    previousSensorExec = NUM_ULTRA_SENSORS - 1;
  else
    previousSensorExec = (this->sensorToExec) - 1;


  /* If the previous sensor is in the moddle of a reading, do not allow other sensors
   *   to begin a new reading. */
  if (this->ptrToUltraSonicObjArray[previousSensorExec]->getReadingInProgress() == true) {
    /* A reading is in progress on sensor, we need to check to make sure there isn't
     *   a timeout while waiting for the echo pulses. */
    if (currentMicrosTime - (this->ptrToUltraSonicObjArray[previousSensorExec]->getFirstEchoTime()) > ECHO_TIMEOUT) {
      this->ptrToUltraSonicObjArray[previousSensorExec]->setReadingInProgress(false);
      this->ptrToUltraSonicObjArray[previousSensorExec]->setInvalidSensorFlag(true);
      return false;
    }
    /* In the middle of a reading, and there is no echo timeout. Return true */
    return true;
  }

  /* The previous sensor is not currently running a reading so begin running an exec
   *   on the next sensor to be polled. Only begin a reading on a sensor, if that
   *   sensor is still valid */
  if (this->ptrToUltraSonicObjArray[sensorToExec]->getInvalidStatus() == false)
    this->ptrToUltraSonicObjArray[sensorToExec]->triggerAPulse();

  /* Increment the next sensor to be polled */
  if (++sensorToExec == NUM_ULTRA_SENSORS)
    sensorToExec = FRONT; //FRONT enum value is 0

  return true;

#undef ECHO_TIMEOUT
}


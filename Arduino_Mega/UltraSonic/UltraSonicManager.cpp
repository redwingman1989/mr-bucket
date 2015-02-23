#include "UltraSonicSensor.h"



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
                                     sensorToExec(0)
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
bool UltraSonicManager::RunTick()
{
  #define ECHO_TIMEOUT (10000)
  uint32_t currentMicrosTime = micros();

  /* For each of the UltraSonic sensors... */
  for (uint8_t sensor = 0; sensor < NUM_ULTRA_SENSORS; i++) {
    /* If the sensor is in the middle of a reading, do not allow other sensors
     *   to begin a new reading. */
    if ((this->(ptrToUltraSonicObjArray+sensor)->getReadingInProgress() == true) {
      /* A reading is in progress on sensor, we need to check to make sure there isn't
       *   a timeout while waiting for the echo pulses. */
      if (currentMicrosTime - this->(ptrToUltraSonicObjArray+sensor)->getFirstEchoTime() > ECHO_TIMEOUT) {
          this->(ptrToUltraSonicObjArray+sensor)->setReadingInProgress(false);
          this->(ptrToUltraSonicObjArray+sensor)->setInvalidSensorFlag(true);
      }
      return;
    }

  }

  /* Begin running an exec on the next sensor to be polled */
  this->(ptrToUltraSonicObjArray+sensorToExec)->triggerAPulse();

  /* Increment the next sensor to be polled */
  if (++sensorToExec == NUM_ULTRA_SENSORS)
    sensorToExec = FRONT;

#undef ECHO_TIMEOUT
}


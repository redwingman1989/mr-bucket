#include "UltraSonicSensor.h"
#include "UltraSonicManager.h"



 /*************************************************************
 * Function:     UltraSonicManager Constructor
 * Input:        void
 * Return:       void
 * Description:  This is the constructor for an UltraSonicManager
 *                 object.
 *************************************************************/
UltraSonicManager::UltraSonicManager() :
                    sensorToExec(0),
                    numManagedSensors(0)
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
 * Function:     addSensor
 * Input:        uint8_t trigerPinValue, uint8_t echoPinValue
 * Return:       void
 * Description:  Adds an UltraSonic Sensor to the list of managed
 *                 UltraSonic Sensors.
 *************************************************************/
void UltraSonicManager::addSensor(uint8_t trigPin, uint8_t echoPin)
{
  /* Create an UltraSonic Sensor */
  UltraSonicSensor sensor = UltraSonicSensor(trigPin, echoPin);

  /* Add to the array of UltraSonic Sensors */
  arrManagedSensors[arrManagedSensors++] = sensor;
}


 /*************************************************************
 * Function:     getSensor
 * Input:        sensor_t sensor
 * Return:       void
 * Description:  Returns the pointer to an UltraSonic Sensor.
 *************************************************************/
UltraSonicSensor * getSensor(sensor_t sensor)
{
  return &(this->arrManagedSensors[sensor]);
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
  #define ECHO_TIMEOUT (100000) // Currently set randomly

  /* previousSensorExec is initialized to the next sensor to exec, it
   *   must be updated before it is used!!!! This way we can initialize
   *   it to a value that is possible to be held in a uint8_t */
  uint8_t previousSensor = this->sensorToExec;
  uint32_t currentMicrosTime = micros();

//  Serial.print("Sensor To Exec: ");
//  Serial.println(this->sensorToExec);

  /* By knowing what the next sensor to exec is, we need to look at the sensor
   *   prior and see if it is in the middle of taking a reading. Handle wrap around. */
  if (this->sensorToExec == 0)
    previousSensor = NUM_ULTRA_SENSORS - 1;
  else
    previousSensor = (this->sensorToExec) - 1;

//  Serial.print("Previous Sensor to Exec: ");
//  Serial.println(previousSensorExec);
//
//  Serial.print("Current Micros: ");
//  Serial.println(currentMicrosTime);
//
//  Serial.print("First Echo Time: ");
//  Serial.println((this->ultraSonicSensors[previousSensorExec]->getFirstEchoTime()));

  /* If the previous sensor is in the moddle of a reading, do not allow other sensors
   *   to begin a new reading. */
  if (this->ultraSonicSensors[previousSensor]->getReadingInProgress() == true) {
      Serial.println("prev sen reading in progress");
    /* A reading is in progress on sensor, we need to check to make sure there isn't
     *   a timeout while waiting for the echo pulses. */
    if (currentMicrosTime - (this->ultraSonicSensors[previousSensor]->getFirstEchoTime()) > ECHO_TIMEOUT) {
//      Serial.println("prev. sen echo timeout");
      this->ultraSonicSensors[previousSensor]->setReadingInProgress(false);
//      this->ultraSonicSensors[previousSensor]->setInvalidSensorFlag(true);
      return false;
    }
    /* In the middle of a reading, and there is no echo timeout. Return true */
    return true;
  }

  /* The previous sensor is not currently running a reading so begin running an exec
   *   on the next sensor to be polled. Only begin a reading on a sensor, if that
   *   sensor is still valid */
  if ((this->ultraSonicSensors[sensorToExec]->sensorInvalid()) == false) {
    if (sensorToExec == FRONT)
      Serial.println("Triggering a pulse!");
    this->ultraSonicSensors[sensorToExec]->triggerAPulse();
  }
  else
    Serial.println("I SHOULD NEVER GET HERE!");

  /* Increment the next sensor to be polled */
  if (++sensorToExec == NUM_ULTRA_SENSORS)
    sensorToExec = FRONT; //FRONT enum value is 0

  Serial.println();

  return true;

#undef ECHO_TIMEOUT
}


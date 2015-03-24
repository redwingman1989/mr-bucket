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
                    sensorToExec(FRONT),
                    numManagedSensors(0)
{
  memset(arrManagedSensors, 0, (sizeof(UltraSonicSensor) * NUM_ULTRA_SENSORS));
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
  arrManagedSensors[numManagedSensors++] = sensor;
}


 /*************************************************************
 * Function:     getSensor
 * Input:        ultSensor_t sensor
 * Return:       void
 * Description:  Returns the pointer to an UltraSonic Sensor.
 *************************************************************/
UltraSonicSensor * UltraSonicManager::getSensor(ultSensor_t sensor)
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
  /* Initialize the last sensor executed to a known good value, then update it */
  ultSensor_t lastSensorExec = sensorToExec;

  /* Rollover */
  if (lastSensorExec == FRONT)
    lastSensorExec = (ultSensor_t)((uint8_t)NUM_ULTRA_SENSORS - 1);

  /* Get a pointer to the UltraSonic Sensor that was last executed */
  UltraSonicSensor * lastSensor = this->getSensor(lastSensorExec);

  /* Get a pointer to the UltraSonic Sensor to exec */
  UltraSonicSensor * sensor = this->getSensor(sensorToExec);

//  /* Check to see if the last sensor finished it's readings. It should have by now. */
//  if (lastSensor->getReadingInProgress() == true) {
//
//  }

  /* Trigger a pulse on the sensor to exec */
  sensor->triggerAPulse();

  return true;

}

#include "Pins.h"
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
  if (trigPin == pinFUltraTrig)
    arrManagedSensors[FRONT] = sensor;
  else if (trigPin == pinLUltraTrig)
    arrManagedSensors[LEFT] = sensor;
  else if (trigPin == pinRUltraTrig)
    arrManagedSensors[RIGHT] = sensor;
  else {
    Serial.println("Error Init. UltraSonic sen. in array of sensors.");
    /* Don't increment the number of Managed sensors, because we had an error
     *   adding this sensor. */
    return;
  }

  /* Keep track of the number of UltraSonic Sensors that are managed. May be useful later. */
  numManagedSensors++;
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
  else
    lastSensorExec = (ultSensor_t)((uint8_t)sensorToExec - 1);

  /* Get a pointer to the UltraSonic Sensor that was last executed */
  UltraSonicSensor * lastSensor = this->getSensor(lastSensorExec);

  /* Get a pointer to the UltraSonic Sensor to exec */
  UltraSonicSensor * sensor = this->getSensor(sensorToExec);

  /* Check to see if the last sensor finished it's readings. It should have by now. */
  if (lastSensor->getReadingInProgress() == true) {
    Serial.println("Still in Progress");

    /* Need to be careful here...This code is probably going to change */
    lastSensor->setReadingInProgress(false);
    /* TO DO: Figure out how to prevent the state machine from getting stuck */
    return false;
  }

  /* Check to see if it is time to perform a distance calculation on the previous sensor */
  if(lastSensor->getTimeForCalcFlag() == true) {
    lastSensor->calculateDistance();
  }

  /* Trigger a pulse on the sensor to exec */
  if (sensor->getReadingInProgress() == false) {
    sensor->triggerAPulse();

  /* Move on to the next sensor */
  if (sensorToExec == (NUM_ULTRA_SENSORS - 1))
    sensorToExec = (ultSensor_t)0;
  else
    sensorToExec = ultSensor_t((uint8_t)sensorToExec+1);
  }

  return true;

}


void UltraSonicManager::DebugOutput(HardwareSerial *serialPort) {
  char dbStr[120] = "\0";

  /* Initialize the last sensor executed to a known good value, then update it */
  ultSensor_t lastSensorExec = sensorToExec;

  /* Rollover */
  if (lastSensorExec == FRONT)
    lastSensorExec = (ultSensor_t)((uint8_t)NUM_ULTRA_SENSORS - 1);
  else
    lastSensorExec = (ultSensor_t)((uint8_t)sensorToExec - 1);

  UltraSonicSensor * sensor     = getSensor(sensorToExec);
  UltraSonicSensor * lastSensor = getSensor(lastSensorExec);
  UltraSonicSensor * frontSen   = getSensor(FRONT);
  UltraSonicSensor * leftSen    = getSensor(LEFT);
  UltraSonicSensor * rightSen   = getSensor(RIGHT);

  switch (debugLevel) {
    case 1:
      sprintf(dbStr,
        "SenToEx: %d\nIn Prog: %d\n1st Time: %ul\n2nd Time: %ul\nDist: %0.2f\n",
        sensorToExec,
        sensor->getReadingInProgress(),
        sensor->getFirstEchoTime(),
        sensor->getLastEchoTime(),
        sensor->getCalculatedDistanceValue()
        );
      break;
    case 2:
      sprintf(dbStr,
        "LastSenEx: %d\nIn Prog: %d\n1st Time: %ul\n2nd Time: %ul\nDist: %0.2f\n",
        lastSensorExec,
        lastSensor->getReadingInProgress(),
        lastSensor->getFirstEchoTime(),
        lastSensor->getLastEchoTime(),
        lastSensor->getCalculatedDistanceValue());
      break;
    case 3:
      sprintf(dbStr,
        "UltFPro: %d\nUltFt1: %lu\nUltFt2: %lu\nUltFDist: %f\n\
        UltLPro: %d\nUltLt1: %lu\nUltLt2: %lu\nUltLDist: %f\n\
        UltRPro: %d\nUltRt1: %lu\nUltRt2: %lu\nUltRDist: %f\n",
        frontSen->getReadingInProgress(),
        frontSen->getFirstEchoTime(),
        frontSen->getLastEchoTime(),
        frontSen->getCalculatedDistanceValue(),
        leftSen->getReadingInProgress(),
        leftSen->getFirstEchoTime(),
        leftSen->getLastEchoTime(),
        leftSen->getCalculatedDistanceValue(),
        rightSen->getReadingInProgress(),
        rightSen->getFirstEchoTime(),
        rightSen->getLastEchoTime(),
        rightSen->getCalculatedDistanceValue());
      break;
    case 4:
      serialPort->print("Center: ");
      serialPort->println(frontSen->getCalculatedDistanceValue());
      serialPort->print("Left:   ");
      serialPort->println(leftSen->getCalculatedDistanceValue());
      serialPort->print("Right:  ");
      serialPort->println(rightSen->getCalculatedDistanceValue());
    default:
      break;
  }
  serialPort->print(dbStr);
};

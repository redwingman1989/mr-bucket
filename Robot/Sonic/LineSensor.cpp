/* Line Sensor Class */
#include <Arduino.h>
#include "LineSensor.h"
//#include "global.h"


/*************************************************************
 * Function:     LineSensor Constructor
 * Input:        unsigned char *
 * Return:       void
 * Description:  This is the Constructor for a LineSensor object. The constructor
 *                 takes a pointer to an array of pin mappings for each sensor on
 *                 the line sensor.
 *                 The constructor uses initialization lists to setup the pin
 *                 values for the objects. We also initialize the time value for
 *                 the start of the cap charging time to 0 (making sure it is not
 *                 a garbage value). Finally, the readings from the line sensor
 *                 are set to 0 (again, preventing a garbage value).
 *                 The readings from the sensor are stored as a byte that can be
 *                 read. The bit values of this byte represent if a black line
 *                 was detected (Bit Value = 1), or if a white surface was detected
 *                 (Bit Value = 0);
 *************************************************************/
LineSensor::LineSensor(int port, int configRegister) :
port(port),
configRegister(configRegister),
startCapChargeTime(0)
{
  this->sensorReadings.allReadings = 0;
}


/*************************************************************
 * Function:     getReading()
 * Return:       void
 * Description:  This function is called after the LineSensor::beginCheck()
 *                 is called. In order to figure out what PORT that the invoking
 *                 object is attached to, the function looks at what the object's
 *                 pinSen1 value. Once the PORT is identified, the value of the
 *                 PORT is stored in the object's sensorReadings member.
 *************************************************************/
void LineSensor::beginCheck()
{
      /* Drive Sensor Line High */
      this->port |= 0xff;

      /* Make the port an output */
      this->configRegister |= 0xff;

      /* Charge the lines for 10 us */
      delayMicroseconds(10);

      /* Make the port an input */
      this->configRegister &= 0;
}


/*************************************************************
 * Function:     getReading()
 * Return:       void
 * Description:  This function is called after the LineSensor::beginCheck()
 *                 is called. In order to figure out what PORT that the invoking
 *                 object is attached to, the function looks at what the object's
 *                 pinSen1 value. Once the PORT is identified, the value of the
 *                 PORT is stored in the object's sensorReadings member.
 *************************************************************/
void LineSensor::getReading()
{
  /* Figure out which PORT this sensor is configured for
   *   and return the value of the PORT to the Line Sensor
   *   object's sensorReadings data member */
    this->sensorReadings.allReadings = PINA;

}

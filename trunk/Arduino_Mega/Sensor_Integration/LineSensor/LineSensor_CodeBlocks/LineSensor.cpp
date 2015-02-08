/* Line Sensor Class */
#include <Arduino.h>
#include "LineSensor.h"
#include "global.h"


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
LineSensor::LineSensor(unsigned char * inPinMap) :
pinMap(inPinMap),
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
  /* Map Pin Value to PORT so we know which PORT to prep */
  switch ((unsigned char) *(this->pinMap)) {
    case PORTA_PIN_0:
      /* Drive Sensor Line High */
      PORTA = PORTA | B11111111;

      /* Make the port an output */
      DDRA = DDRA | B11111111;

      /* Charge the lines for 10 us */
      delayMicroseconds(10);

      /* Make the port an input */
      DDRA = DDRA & B00000000;

      break;

    case PORTB_PIN_0:
      /* Drive Sensor Line High */
      PORTB = PORTB | B11111111;

      /* Make the port an output */
      DDRB = DDRB | B11111111;

      /* Charge the lines for 10 us */
      delayMicroseconds(10);

      /* Make the port an input */
      DDRB = DDRB & B00000000;

      break;

    case PORTC_PIN_0:
      /* Drive Sensor Line High */
      PORTC = PORTC | B11111111;

      /* Make the port an output */
      DDRC = DDRC | B11111111;

      /* Charge the lines for 10 us */
      delayMicroseconds(10);

      /* Make the port an input */
      DDRC = DDRC & B00000000;
      break;

    case PORTL_PIN_0:
      /* Drive Sensor Line High */
      PORTL = PORTL | B11111111;

      /* Make the port an output */
      DDRL = DDRL | B11111111;

      /* Charge the lines for 10 us */
      delayMicroseconds(10);

      /* Make the port an input */
      DDRL = DDRL & B00000000;
      break;

    default:
      /* Print out a debug message if debug build */
      if (DEBUG_BUILD == TRUE)
        Serial.println("Invalid Line Sensor Port!");
      break;

      /* TO DO: Perhaps invalidate the sensor? Not sure how we could recover.
       *          would need to add a new member to the class. */
  }
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
  if ((*this->pinMap) == PORTA_PIN_0)
    this->sensorReadings.allReadings = PINA;

  else if ((*this->pinMap) == PORTB_PIN_0)
    this->sensorReadings.allReadings = PINB;

  else if ((*this->pinMap) == PORTC_PIN_0)
    this->sensorReadings.allReadings = PINC;

  else if ((*this->pinMap) == PORTL_PIN_0)
    this->sensorReadings.allReadings = PINL;

  else {
    /* Print out a debug message if debug build */
    if (DEBUG_BUILD == TRUE)
      Serial.println("takeReading() Invalid Line Sensor Port!");

    /* TO DO: Perhaps invalidate the sensor? Not sure how we could recover.
     *          would need to add new member to the class. */
  }

}

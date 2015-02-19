/* Line Sensor Class */
#include <Arduino.h>
#include "LineSensor.h"


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
LineSensor::LineSensor(const uint8_t * inPinMap) :
pinMap(inPinMap),
startCapChargeTime(0)
{
  /* Map the PORT to the member */
  if ((uint8_t)(*this->pinMap) == (uint8_t)PORTA_PIN_0) {
    /* Pin 0 Value matches to PORTA */
    this->ptrPortDataReg    = (uint8_t *)PORTA_DATA_REG;
    this->ptrPortDataDirReg = (uint8_t *)PORTA_DATA_DIR_REG;
    this->ptrPortInputPins  = (uint8_t *)PORTA_IN_PINS_REG;
  }
  else if ((uint8_t) (*this->pinMap) == (uint8_t)PORTB_PIN_0) {
    /* Pin 0 Value matches to PORTB */
    this->ptrPortDataReg    = (uint8_t *)PORTB_DATA_REG;
    this->ptrPortDataDirReg = (uint8_t *)PORTB_DATA_DIR_REG;
    this->ptrPortInputPins  = (uint8_t *)PORTB_IN_PINS_REG;
  }
  else if ((uint8_t)(*this->pinMap) == (uint8_t)PORTC_PIN_0) {
    /* Pin 0 Value matches to PORTC */
    this->ptrPortDataReg    = (uint8_t *)PORTC_DATA_REG;
    this->ptrPortDataDirReg = (uint8_t *)PORTC_DATA_DIR_REG;
    this->ptrPortInputPins  = (uint8_t *)PORTC_IN_PINS_REG;
  }
  else if ((uint8_t)(*this->pinMap) == (uint8_t)PORTL_PIN_0) {
    /* Pin 0 Value matches to PORTL */
    this->ptrPortDataReg    = (uint8_t *)PORTL_DATA_REG;
    this->ptrPortDataDirReg = (uint8_t *)PORTL_DATA_DIR_REG;
    this->ptrPortInputPins  = (uint8_t *)PORTL_IN_PINS_REG;
  }
  else {
    Serial.println("Invalid LineSensor pin to PORT Mapping.");

    this->ptrPortDataReg    = NULL;
    this->ptrPortDataDirReg = NULL;
    this->ptrPortInputPins  = NULL;
  }

  /* Set all readings to 0. (Remove any garbage value) */
  this->sensorReadings.allReadings = 0;
}


/*************************************************************
 * Function:     LineSensor Destructor
 * Input:        void
 * Return:       void
 * Description:  This is the Destructor for a LineSensor object.
 *************************************************************/
LineSensor::~LineSensor()
{
    free(this);
}


/*************************************************************
 * Function:     startCharging()
 * Parameters:   void
 * Return:       void
 * Description:  This function charges the sensors
 *************************************************************/
void LineSensor::startCharging()
{
  /* Drive Sensor Line High */
  *(this->ptrPortDataReg) |= 0xFF; //B11111111

  /* Make the port an output */
  *(this->ptrPortDataDirReg) |= 0xFF; //B11111111
}


/*************************************************************
 * Function:     stopCharging()
 * Parameters:   void
 * Return:       void
 * Description:  This function stops charging the sensors
 *               and sets up for reading
 *************************************************************/
void LineSensor::stopCharging()
{
  /* Make the port an input */
  *(this->ptrPortDataDirReg) &= 0x00;

  /* Mark the start time of the reading */
  this->startCapChargeTime = micros();
}


/*************************************************************
 * Function:     takeReading()
 * Parameter(s): void
 * Return:       void
 * Description:  This function is called after the LineSensor::beginCheck()
 *                 is called. In order to figure out what PORT that the invoking
 *                 object is attached to, the function looks at what the object's
 *                 pinSen1 value. Once the PORT is identified, the value of the
 *                 PORT is stored in the object's sensorReadings member.
 *************************************************************/
void LineSensor::takeReading()
{
  /* Look at the Line Sensor's Input Pins */
   if (this->ptrPortInputPins != NULL)
     this->sensorReadings.allReadings = (uint8_t)*this->ptrPortInputPins;
  else {
    /* Print out a debug message if debug build */
    Serial.println("Line Sensor Input Pins Pointer is NULL!");

    /* TO DO: Perhaps invalidate the sensor? Not sure how we could recover.
     *          would need to add new member to the class. */
}
}


/*************************************************************
 * Function:     getLineSensorReadings()
 * Return:       void
 * Description:  This function is called after the LineSensor::beginCheck()
 *                 is called. In order to figure out what PORT that the invoking
 *                 object is attached to, the function looks at what the object's
 *                 pinSen1 value. Once the PORT is identified, the value of the
 *                 PORT is stored in the object's sensorReadings member.
 *************************************************************/
uint8_t LineSensor::getLineSensorReadings() {
  return this->sensorReadings.allReadings;
}

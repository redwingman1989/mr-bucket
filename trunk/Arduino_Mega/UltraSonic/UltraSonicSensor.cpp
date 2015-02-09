#include <Arduino.h>
#include "global.h"
#include "UltraSonicSensor.h"

/*************************************************************
 * Function:     UltraSonicSensor Constructor
 * Input:        uint8_t triggerPin
                 uint8_t echoPin
 * Return:       void
 * Description:  This is the constructor for an UltraSonicSensor
 *                 object. The constructor takes a few parameters.
 *                 The first parameter is the pin number of the
 *                 UltraSonic's trigger signal. The second parameter
 *                 is the pin number of the UltraSonic's echo signal.
 *************************************************************/
 UltraSonicSensor::UltraSonicSensor(uint8_t triggerPin,
                                    uint8_t echoPin):
                                    pinTrigger(triggerPin),
                                    pinEcho(echoPin),
                                    triggerTime(0)
{

}

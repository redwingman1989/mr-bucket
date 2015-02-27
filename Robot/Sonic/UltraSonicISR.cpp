#include "UltraSonicISR.h"
#include "UltraSonicSensor.h"


/*************************************************************
* Function:     frontUltraSonicISR
* Input:        void
* Return:       void
* Description:  ISR for the front UltraSonic sensor.
*************************************************************/
void frontUltraSonicISR()
{
  UltraSonicSensor * sensor = ultraSonicMgr.getSensor(FRONT);

  if (digitalRead(sensor->getEchoPin()))
    sensor->setFirstEchoTime((uint32_t) micros());
  else {
    sensor->setLastEchoTime((uint32_t) micros());
    sensor->setReadyForDistanceCalc(true);
    sensor->setReadingInProgress(false);
    ultraSonicMgr.setTimeOut(0);
  } /* else */
} /* frontUltraSonicISR() */


/*************************************************************
* Function:     leftUltraSonicISR
* Input:        void
* Return:       void
* Description:  ISR for the left UltraSonic sensor.
*************************************************************/
void leftUltraSonicISR()
{
  UltraSonicSensor * sensor = ultraSonicMgr.getSensor(LEFT);

  if (digitalRead(sensor->getEchoPin()))
    sensor->setFirstEchoTime((uint32_t) micros());
  else {
    sensor->setLastEchoTime((uint32_t) micros());
    sensor->setReadyForDistanceCalc(true);
    sensor->setReadingInProgress(false);
  } /* else */
} /* lefttUltraSonicISR() */


/*************************************************************
* Function:     rightUltraSonicISR
* Input:        void
* Return:       void
* Description:  ISR for the right UltraSonic sensor.
*************************************************************/
void rightUltraSonicISR()
{
  UltraSonicSensor * sensor = ultraSonicMgr.getSensor(RIGHT);

  if (digitalRead(sensor->getEchoPin()))
    sensor->setFirstEchoTime((uint32_t) micros());
  else {
    sensor->setLastEchoTime((uint32_t) micros());
    sensor->setReadyForDistanceCalc(true);
    sensor->setReadingInProgress(false);
  } /* else */
} /* rightUltraSonicISR() */

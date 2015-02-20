#include "UltraSonicISR.h"
#include "UltraSonicSensor.h"
#include "UltraSonic_ino_header.h"

/*************************************************************
* Function:     frontUltraSonicISR
* Input:        void
* Return:       void
* Description:  ISR for the front UltraSonic sensor.
*************************************************************/
void frontUltraSonicISR()
{
    Serial.println("Called");
    if (digitalRead(ultraSonicFront.getTriggerPin()))
        ultraSonicFront.setFirstEchoTime((uint32_t) micros());
    else {
        ultraSonicFront.setLastEchoTime((uint32_t) micros());
        ultraSonicFront.setReadyForDistanceCalc(true);
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
    if (digitalRead(ultraSonicLeft.getTriggerPin()))
        ultraSonicLeft.setFirstEchoTime((uint32_t) micros());
    else {
        ultraSonicLeft.setLastEchoTime((uint32_t) micros());
        ultraSonicLeft.setReadyForDistanceCalc(true);
    } /* else */
} /* frontUltraSonicISR() */


/*************************************************************
* Function:     rightUltraSonicISR
* Input:        void
* Return:       void
* Description:  ISR for the right UltraSonic sensor.
*************************************************************/
void rightUltraSonicISR()
{
    if (digitalRead(ultraSonicRight.getTriggerPin()))
        ultraSonicRight.setFirstEchoTime((uint32_t) micros());
    else {
        ultraSonicRight.setLastEchoTime((uint32_t) micros());
        ultraSonicRight.setReadyForDistanceCalc(true);
    } /* else */
} /* frontUltraSonicISR() */

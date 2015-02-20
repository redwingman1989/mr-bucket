#include <Arduino.h>
#include "UltraSonicSensor.h"
#include "Interrupts.h"

 /*************************************************************
 * Function:     UltraSonicSensor Constructor
 * Input:        uint8_t * inPinMap
 * Return:       void
 * Description:  This is the constructor for an UltraSonicSensor
 *                 object. The constructor takes a pointer to an
 *                 array that contains the Trigger pin and the
 *                 Echo pin values for the UltraSonicSensor object.
 *************************************************************/
 UltraSonicSensor::UltraSonicSensor(const uint8_t * inPinMap):
                                    triggerStartTime(0),
                                    rawDataArrayIdx(0),
                                    rxFirstEchoTime(0),
                                    rxLastEchoTime(0),
                                    timeToCalculateDistance(false)
{
    /* Save a local variable to prevent calling getTriggerPin 3 times */
    uint8_t triggerPin = this->getTriggerPin();

    /* Assign the Pin Map pointer */
    this->pinMap = inPinMap;

    /* Initialize the Raw Data Array */
    memset(this->rawDataArray, 0, sizeof(uint8_t) * NUM_ULTRA_FILT_READINGS);

    /* Attach the motherfucking interrupt
     * You cannot attach Class Methods as the ISR to call. So I had to make
     * the ISR a regular function outside of the class */
//    if (triggerPin == ULTRA_FRONT_ECHO_PIN) {
//      attachInterrupt(EXT_INTERRUPT_3, echoISR, CHANGE);
//      this->extInterruptNumber = EXT_INTERRUPT_3;
//    }
//    else if (triggerPin == ULTRA_LEFT_ECHO_PIN) {
//      attachInterrupt(EXT_INTERRUPT_2, echoISR, CHANGE);
//      this->extInterruptNumber = EXT_INTERRUPT_2;
//    }
//    else if (triggerPin == ULTRA_RIGHT_ECHO_PIN) {
//      attachInterrupt(EXT_INTERRUPT_4, echoISR, CHANGE);
//      this->extInterruptNumber = EXT_INTERRUPT_4;
//    }
//    else {
//      /* Write out some sort of warning. Need to implement this. */
//      this->DebugOutput(&Serial);
//    }

}


 /*************************************************************
 * Function:     UltraSonicSensor Destructor
 * Input:        void
 * Return:       void
 * Description:  Destructor for an UltraSonicSensor object
 *************************************************************/
 UltraSonicSensor::~UltraSonicSensor()
 {
     free(this);
 }

// /*************************************************************
// * Function:     initEchoInterrupt
// * Input:        void
// * Return:       void
// * Description:  Initializes the external interrupt registers.
// *                 This function only needs to be called once.
// *************************************************************/
//void UltraSonicSensor::initEchoInterrupt()
//{
//    /* Pointer to the appropriate External Interrupt Control Register */
//    uint8_t * ptrExtIntCtrlReg;
//
//    /* Clear the global interrupt flag to ensure we don't trigger
//     *   an interrupt while configuring the interrupt.
//     *   RESET THE GLOBAL INERRUPT FLAG BEFORE LEAVING THIS FUNCTION! */
//    cli();
//
//    /* Set the proper External Interrupt Control Register. Two bits
//     *   per external interrupt are used to configure what triggers
//     *   the external interrupt (See ATMEL ATmega 2560 Documentation).
//     *   For our purposes with the UltraSonic Sensor we need to set the
//     *   value for the external interrupt to: "0 1" (0 is the MSB bit)  */
//    if (this->extInterruptNumber <= EXT_INTERRUPT_3) {
//        /* This Interrupt is configured with External Interrupt Control Register A (EICRA) */
//        ptrExtIntCtrlReg = (uint8_t *)ADDR_EXT_INT_CTRL_REG_A;
//    }
//    else if (this->extInterruptNumber < NUM_EXT_INTERRUPTS) {
//        /* This Interrupt is configured with External Interrupt Control Register B (EICRB) */
//        ptrExtIntCtrlReg = (uint8_t *)ADDR_EXT_INT_CTRL_REG_B;
//    }
//
//    /* Configure the External Interrupt Control Register */
//    configureExtIntCtrlRegister(ptrExtIntCtrlReg);
//
//    /* Set the proper External Interrupt Mask Register (EIMSK) bit */
//    EIMSK |= ((uint8_t)0x01 << (this->extInterruptNumber));
//
//    /* Make sure the global interrupt flag is set */
//    sei();
//}
//
//
// /*************************************************************
// * Function:     configureExtIntCtrlRegister
// * Input:        uint8_t * externalInterruptRegister
// * Return:       void
// * Description:  Configures the appropriate External Interrupt Control Register
// *************************************************************/
//void UltraSonicSensor::configureExtIntCtrlRegister(uint8_t * ptrRegister) {
//    /* Macro for the number of configuration bits per external interrupt
//     *   for the External Interrupt Control Register. */
//    #define NUM_CFG_BITS_PER_EXT_INTERRUPT (2)
//
//    /* Save off the number of interrupts that can be configured per byte */
//    uint8_t interruptsPerByte = NUM_EXT_INTERRUPTS / NUM_CFG_BITS_PER_EXT_INTERRUPT;
//
//    /* Save off the registers value into a local, temp variable */
//    uint8_t newReisterValue = (*ptrRegister);
//
//    /* Bit Shift amount */
//    uint8_t bitShift = 0;
//
//    /* Temporary byte */
//    uint8_t clearBitMaskingByte = 0;
//
//    /* New Register value */
//    uint8_t newRegisterValue = 0;
//
//    /* To calculate the bits that need to be configured. The forumla is as follows:
//     *   (External Interrupt Number % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT = bit shift of the LSb  */
//    bitShift = ((this->extInterruptNumber) % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT;
//
//    /* Set the least significant control register bit for the appropriate interrupt */
//    newRegisterValue |= ((uint8_t) 0x01 << bitShift);
//
//    /* Clear the most significant control register bit for the appropriate interrupt.
//     *   Bit shift ones into the byte (except in the spot that needs to be cleared).
//     *   This leaves a 0 in the bit spot that needs to be cleared in the EICRX register.
//     *   AND logic then can be applied to clear the appropriate bit while preserving the
//     *   value of the other bits */
//    for (uint8_t i = 0; i < EXT_INTERRUPT_7; i++) {
//      if (i != (bitShift + 1))
//        clearBitMaskingByte |= (0x01 << i);
//    }
//
//    /* Use AND logic to clear the requested bit */
//    newRegisterValue &= (clearBitMaskingByte);
//
//    /* Set the Register to teh new register value */
//    (*ptrRegister) = newRegisterValue;
//
//    #undef NUM_CFG_BITS_PER_EXT_INTERRUPT
//}


 /*************************************************************
 * Function:     setFirstEchoTime()
 * Parameter:    uint32_t timeInMicroseconds
 * Return:       void
 * Description:  Set the class member variable for the time in
 *                 microseconds of the first received echo pulse.
 *************************************************************/
void UltraSonicSensor::setFirstEchoTime(uint32_t timeInMicroseconds)
{
    rxFirstEchoTime = timeInMicroseconds;
}


 /*************************************************************
 * Function:     setLastEchoTime()
 * Parameter:    uint32_t timeInMicroseconds
 * Return:       void
 * Description:  Set the class member variable for the time in
 *                 microseconds of the last received echo pulse.
 *************************************************************/
void UltraSonicSensor::setLastEchoTime(uint32_t timeInMicroseconds)
{
    rxLastEchoTime = timeInMicroseconds;
}


 /*************************************************************
 * Function:     setReadyForDistanceCalc()
 * Parameter:    bool timeForDistanceCalc
 * Return:       void
 * Description:  Set the class member variable that wil be used to
 *                 see if it is time to calculate distances.
 *************************************************************/
void UltraSonicSensor::setReadyForDistanceCalc(bool timeForDistanceCalc)
{
    timeToCalculateDistance = timeForDistanceCalc;
}


 /*************************************************************
 * Function:     triggerAPulse()
 * Parameter:    void
 * Return:       void
 * Description:  Sets the trigger pin high for 10 microseconds.
 *                 This triggers an ultrasonic wave to be produced
 *                 by the ultrasonic sensor.
 *************************************************************/
void UltraSonicSensor::triggerAPulse()
{
    Serial.println(*(this->pinMap+TRIGGER_PIN));
    digitalWrite((uint8_t) *(this->pinMap+TRIGGER_PIN), HIGH);
    delayMicroseconds(10);
    digitalWrite((uint8_t) *(this->pinMap+TRIGGER_PIN), LOW);
}


// /*************************************************************
// * Function:     getFirstEchoTime()
// * Parameter:    void
// * Return:       uint32_t timeInMicroseconds
// * Description:  Get the class member variable for the time in
// *                 microseconds of the first received echo pulse.
// *************************************************************/
//uint32_t UltraSonicSensor::getFirstEchoTime(void)
//{
//    return this->rxFirstEchoTime;
//}


// /*************************************************************
// * Function:     getTrigPinHighDuration
// * Input:        void
// * Return:       uint32_t timeInMicroseconds
// * Description:  Returns the duration, in microseconds, the the
// *                 Trigger pin was high for. This value is used
// *                 in the distance calculation.
// *************************************************************/
//uint32_t UltraSonicSensor::getTrigPinHighDuration()
//{
//    return this->trigPinHighDuration;
//}


 /*************************************************************
 * Function:     getTriggerPin
 * Input:        void
 * Return:       uint8_t triggerPin
 * Description:  Returns the value of the UltraSonicSensor
 *                 object's Trigger pin.
 *************************************************************/
uint8_t UltraSonicSensor::getTriggerPin()
{
    return ((uint8_t) *(this->pinMap+TRIGGER_PIN));
}


 /*************************************************************
 * Function:     getEchoPin
 * Input:        void
 * Return:       uint8_t echoPin
 * Description:  Returns the value of the UltraSonicSensor
 *                 object's Echo pin.
 *************************************************************/
uint8_t UltraSonicSensor::getEchoPin()
{
    return ((uint8_t) *(this->pinMap+ECHO_PIN));
}


 /*************************************************************
 * Function:     getTimeForCalcFlag
 * Input:        void
 * Return:       uint8_t echoPin
 * Description:  Returns the value of the UltraSonicSensor
 *                 object's Echo pin.
 *************************************************************/
bool UltraSonicSensor::getTimeForCalcFlag()
{
    return ((bool)this->timeToCalculateDistance);
}


 /*************************************************************
 * Function:     getFirstEchoTime
 * Input:        void
 * Return:       uint8_t echoPin
 * Description:  Returns the time of the first echo pulse.
 *************************************************************/
uint32_t UltraSonicSensor::getFirstEchoTime()
{
    return ((uint32_t)this->rxFirstEchoTime);
}


// /*************************************************************
// * Function:     echoISR
// * Input:        void
// * Return:       void
// * Description:  The ISR that is called when the Echo pin transitions from
// *                 low to high or high to low.
// *************************************************************/
//void UltraSonicSensor::echoISR()
//{
//    // Allowing for 16 feet of total travel time
//    // ~340 meters per second = V_ultrasonic_pulse
//    // t = ~0.014343529 second
////    #define ECHO_TIMEOUT_IN_MILLISECONDS (14350)
//
//    /* If the Pin is high that means the first echo hit the sensor */
////    if (digitalRead((uint8_t)this->pinMap+ECHO_PIN))
////        this->rxFirstEchoTime = (uint32_t) micros();
//    /* Else the Pin went high to low and triggered the ISR
//     *   this indicates that the last pulse hit the sensor */
//    else {
//        /* Calculate how long the ISR pin was high */
//        this->rxLastEchoTime = (uint32_t) micros();
//
//        /* Flag that it is time to run the distance calculations outside of the ISR */
//        this->timeToCalculateDistance = true;
//
////        this->rawDataArray[rawDataArrayIdx] = this->calculateDistance();
//    }
//
//
////    #undef ECHO_TIMEOUT_IN_MILLISECONDS
//}


// /*************************************************************
// * Function:     calculateDistance
// * Input:        void
// * Return:       void
// * Description:  This function calculates the disatance to the
// *                 reflected object/surface, based on the duration
// *                 of time the Trigger pin was set high.
// *           According to the datasheet:
// *             range = high level time * velocity of Ultrasonic Wave / 2
// *
// *           - Velocity of the UltraSonic Wave = ~ 340 meters/second
// *                                             = ~ 13,386 inches/second
// *                                             = ~ 0.013386 inches/microsecond
// *************************************************************/
//inline uint8_t UltraSonicSensor::calculateDistance()
//{
//    #define VELOCITY_ULTRA_WAVE_IN_MICROSECONDS (0.013386)
//
//    return (uint8_t)((float)(this->rxLastEchoTime - this->rxFirstEchoTime)) *
//                      (float) VELOCITY_ULTRA_WAVE_IN_MICROSECONDS / 2.0;
//
//    #undef VELOCITY_ULTRA_WAVE_IN_MICROSECONDS
//}


 /*************************************************************
 * Function:     RunTick()
 * Parameter:    void
 * Return:       void
 * Description:  This is a virtual function inherited from the parent class:
 *                 RunableModule.
 *************************************************************/
bool UltraSonicSensor::RunTick(uint16_t time,RobotState state)
{
  /* Need to implement */
  return true;
}


 /*************************************************************
 * Function:     DebugOutput()
 * Parameter:    void
 * Return:       void
 * Description:  This is a virtual function inherited from the parent class:
 *                 RunableModule.
 *************************************************************/
void UltraSonicSensor::DebugOutput(HardwareSerial *serialPort)
{
    return;
}

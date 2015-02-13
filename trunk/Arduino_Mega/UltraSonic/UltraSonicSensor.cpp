#include <Arduino.h>
#include "global.h"
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
                                    triggerStartTime(0)
{
    /* Assign the Pin Map pointer */
    this->pinMap = inPinMap;

    /* Map the Echo pin number to the external interrupt */
    if (((uint8_t) (*this->pinMap+ECHO_PIN)) == ULTRA_FRONT_ECHO_PIN)
        this->extInterruptNumber = (uint8_t) EXT_INTERRUPT_3;
    else if ((uint8_t) (*this->pinMap+ECHO_PIN)) == ULTRA_LEFT_ECHO_PIN)
        this->extInterruptNumber = (uint8_t) EXT_INTERRUPT_2;
    else if ((uint8_t) (*this->pinMap+ECHO_PIN)) == ULTRA_RIGHT_ECHO_PIN)
        this->extInterruptNumber = (uint8_t) EXT_INTERRUPT_4;
    else {
        if (DEBUG_BUILD)
            Serial.println("UltraSonic Sensor invalid external pin map.");
        /* Set to an interrupt number that is outside the range of possible
         *   interrupts. */
        this->extInterruptNumber = NUM_EXT_INTERRUPTS;
    }

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

/*************************************************************
 * Function:     initEchoInterrupt
 * Input:        void
 * Return:       void
 * Description:  Initializes the external interrupt registers.
 *                 This function only needs to be called once.
 *************************************************************/
void UltraSonicSensor::initEchoInterrupt()
{
    /* Macro for the number of configuration bits per external interrupt
     *   for the External Interrupt Control Register. */
    #define NUM_CFG_BITS_PER_EXT_INTERRUPT (2)

    /* Save off the number of interrupts that can be configured per byte */
    uint8_t interruptsPerByte = NUM_EXT_INTERRUPTS / NUM_CFG_BITS_PER_EXT_INTERRUPT;

    /* Bit Shift amount */
    uint8_t bitShift = 0;

    /* Clear the global interrupt flag to ensure we don't trigger
     *   an interrupt while configuring the interrupt.
     *   RESET THE GLOBAL INERRUPT FLAG BEFORE LEAVING THIS FUNCTION! */
    cli();

    /* Set the proper External Interrupt Control Register. Two bits
     *   per external interrupt are used to configure what triggers
     *   the external interrupt (See ATMEL ATmega 2560 Documentation).
     *   For our purposes with the UltraSonic Sensor we need to set the
     *   value for the external interrupt to: "0 1" (0 is the MSB bit)  */
    if (this->extInterruptNumber <= EXT_INTERRUPT_3) {
        /* This Interrupt is configured with External Interrupt Control Register A (EICRA) */

        /* To calculate the bits that need to be configured. The forumla is as follows:
         *   (External Interrupt Number % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT = bit shift of the LSb  */
        bitShift = ((this->extInterruptNumber) % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT

        EICRA |= ((uint8_t) 0x01 << bitShift);


        /* NEED TO 0 out the bitShift + 1 bit. */




    }
    else if (this->extInterruptNumber < NUM_EXT_INTERRUPTS) {
        /* This Interrupt is configured with External Interrupt Control Register B (EICRB) */
    }

    /* Set the proper External Interrupt Mask Register bit */
    ENABLE_EXT_INTERRUPT(this->extInterruptNumber);

    /* Make sure the global interrupt flag is set */
    sei();

    #undef NUM_CFG_BITS_PER_EXT_INTERRUPT
}


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

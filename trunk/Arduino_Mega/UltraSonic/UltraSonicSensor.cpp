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
    else if ((uint8_t) (*this->pinMap+ECHO_PIN) == ULTRA_LEFT_ECHO_PIN)
        this->extInterruptNumber = (uint8_t) EXT_INTERRUPT_2;
    else if ((uint8_t) (*this->pinMap+ECHO_PIN) == ULTRA_RIGHT_ECHO_PIN)
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
    /* Pointer to the appropriate External Interrupt Control Register */
    uint8_t * ptrExtIntCtrlReg;

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
        ptrExtIntCtrlReg = (uint8_t *)ADDR_EXT_INT_CTRL_REG_A;
    }
    else if (this->extInterruptNumber < NUM_EXT_INTERRUPTS) {
        /* This Interrupt is configured with External Interrupt Control Register B (EICRB) */
        ptrExtIntCtrlReg = (uint8_t *)ADDR_EXT_INT_CTRL_REG_B;
    }

    /* Configure the External Interrupt Control Register */
    configureExtIntCtrlRegister(ptrExtIntCtrlReg);

    /* Set the proper External Interrupt Mask Register (EIMSK) bit */
    EIMSK |= ((uint8_t)0x01 << (this->extInterruptNumber));

    /* Make sure the global interrupt flag is set */
    sei();
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


/*************************************************************
 * Function:     configureExtIntCtrlRegister
 * Input:        uint8_t * externalInterruptRegister
 * Return:       void
 * Description:  Configures the appropriate External Interrupt Control Register
 *************************************************************/
void UltraSonicSensor::configureExtIntCtrlRegister(uint8_t * ptrRegister) {
    /* Macro for the number of configuration bits per external interrupt
     *   for the External Interrupt Control Register. */
    #define NUM_CFG_BITS_PER_EXT_INTERRUPT (2)

    /* Save off the number of interrupts that can be configured per byte */
    uint8_t interruptsPerByte = NUM_EXT_INTERRUPTS / NUM_CFG_BITS_PER_EXT_INTERRUPT;

    /* Save off the registers value into a local, temp variable */
    uint8_t newReisterValue = (*ptrRegister);

    /* Bit Shift amount */
    uint8_t bitShift = 0;

    /* Temporary byte */
    uint8_t clearBitMaskingByte = 0;

    /* New Register value */
    uint8_t newRegisterValue = 0;

    /* To calculate the bits that need to be configured. The forumla is as follows:
     *   (External Interrupt Number % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT = bit shift of the LSb  */
    bitShift = ((this->extInterruptNumber) % interruptsPerByte) * NUM_CFG_BITS_PER_EXT_INTERRUPT;

    /* Set the least significant control register bit for the appropriate interrupt */
    newRegisterValue |= ((uint8_t) 0x01 << bitShift);

    /* Clear the most significant control register bit for the appropriate interrupt.
     *   Bit shift ones into the byte (except in the spot that needs to be cleared).
     *   This leaves a 0 in the bit spot that needs to be cleared in the EICRX register.
     *   AND logic then can be applied to clear the appropriate bit while preserving the
     *   value of the other bits */
    for (uint8_t i = 0; i < EXT_INTERRUPT_7; i++) {
      if (i != (bitShift + 1))
        clearBitMaskingByte |= (0x01 << i);
    }

    /* Use AND logic to clear the requested bit */
    newRegisterValue &= (clearBitMaskingByte);

    /* Set the Register to teh new register value */
    (*ptrRegister) = newRegisterValue;

    #undef NUM_CFG_BITS_PER_EXT_INTERRUPT
}

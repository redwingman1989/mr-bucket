#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

/* Used to set up the External Interrupt Registers manually */
// /* Register Addresses for the External Interrupt Control Registers */
//#define ADDR_EXT_INT_CTRL_REG_A (0x69)
//#define ADDR_EXT_INT_CTRL_REG_B (0x6A)





/* Enumeration */
enum externalInterrupts {
    EXT_INTERRUPT_0,

    EXT_INTERRUPT_1,

    /* External Interrupt 2 = Left UltraSonic Sensor */
    EXT_INTERRUPT_2,
    ULTRA_LEFT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_2,

    /* External Interrupt 3 = Front UltraSonic Sensor */
    EXT_INTERRUPT_3,
    ULTRA_FRONT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_3,

    /* External Interrupt 4 = Right UltraSonic Sensor */
    EXT_INTERRUPT_4,
    ULTRA_RIGHT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_4,

    EXT_INTERRUPT_5,

    EXT_INTERRUPT_6,

    EXT_INTERRUPT_7,

    NUM_EXT_INTERRUPTS
};

#endif // _INTERRUPTS_H

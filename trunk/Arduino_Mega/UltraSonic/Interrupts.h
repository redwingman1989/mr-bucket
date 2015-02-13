#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

/* Register Addresses for the External Interrupt Control Registers */
#define ADDR_EXT_INT_CTRL_REG_A (0x69)
#define ADDR_EXT_INT_CTRL_REG_B (0x6A)

/* Enumeration */
enum externalInterrupts {
    EXT_INTERRUPT_0,
    EXT_INTERRUPT_1,
    EXT_INTERRUPT_2,
    EXT_INTERRUPT_3,
    EXT_INTERRUPT_4,
    EXT_INTERRUPT_5,
    EXT_INTERRUPT_6,
    EXT_INTERRUPT_7,
    NUM_EXT_INTERRUPTS
};

#endif // _INTERRUPTS_H

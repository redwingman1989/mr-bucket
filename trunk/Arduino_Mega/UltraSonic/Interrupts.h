#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H


/* Macro Functions */

/* Enable External Interrupt
 *   Global Interrupts must be enabled as well */
#define ENABLE_EXT_INTERRUPT (interrupt) \
  do {                                       \
    EIMSK |= ((uint8_t)0x01 << interrupt);            \
  } while (0)


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

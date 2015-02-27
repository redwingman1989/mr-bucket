#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

/* Enumeration */
enum externalInterrupts {
    EXT_INTERRUPT_0,
    /* External Interrupt 0 = Right UltraSonic Sensor */
    ULTRA_RIGHT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_0,

    EXT_INTERRUPT_1,

    EXT_INTERRUPT_2,

    EXT_INTERRUPT_3,

    EXT_INTERRUPT_4,
    /* External Interrupt 4 = Left UltraSonic Sensor */
    ULTRA_LEFT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_4,

    EXT_INTERRUPT_5,
    /* External Interrupt 5 = Front UltraSonic Sensor */
    ULTRA_FRONT_EXT_INTERRUPT_NUM = EXT_INTERRUPT_5,

    EXT_INTERRUPT_6,

    EXT_INTERRUPT_7,

    NUM_EXT_INTERRUPTS
};

#endif // _INTERRUPTS_H

#ifndef _SETUP_H
#define _SETUP_H

void setupTimer1Int(unsigned long periodInMicroSecs);

void setupUltraSonicInterrupts();

void setupSerial(unsigned long baud);

void setupPinModes();

#endif // _SETUP_H

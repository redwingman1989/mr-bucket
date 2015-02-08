#ifndef _SETUP_H
#define _SETUP_H

void setupTimer1Int(unsigned long periodInMicroSecs);

void setupTimer3Int(unsigned long periodInMicroSecs);

void setupSerial(unsigned long baud);

void setupLineSensor();

void setupPinModes();

#endif // _SETUP_H

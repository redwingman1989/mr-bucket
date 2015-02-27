#ifndef _ULTRASONICISR_H
#define _ULTRASONICISR_H

#include "UltraSonicManager.h"

void frontUltraSonicISR();
void leftUltraSonicISR();
void rightUltraSonicISR();

extern UltraSonicManager ultraSonicMgr;

#endif // _ULTRASONICISR_H

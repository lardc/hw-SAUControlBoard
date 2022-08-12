#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Functions
//
void LL_ToggleBoardLED();
void LL_SetStateExtLed(bool State);
void LL_SetStateFan(bool State);
void LL_SetStateExtLineSync1(bool State);
void LL_SetStateExtLineSync2(bool State);
void LL_SetStateIntLineSync1(bool State);
void LL_SetStateIntLineSync2(bool State);

bool LL_GetStateIntLineSync1();
bool LL_GetStateIntLineSync2();

#endif //__LOWLEVEL_H

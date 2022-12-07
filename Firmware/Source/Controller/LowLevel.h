#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Functions
void LL_ToggleBoardLED();
void LL_INT_FAN(bool State);
void LL_ExternalLampGREEN(bool State);
void LL_ExternalLampRED(bool State);
void LL_ButtonPC(bool State);

#endif //__LOWLEVEL_H

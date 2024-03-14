#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Types
//
typedef enum _SwitchLamp
{
	SW_Lamp1	= 0,
	SW_Lamp2	= 1,
	SW_Lamp3	= 2,
}SwitchLamp;

typedef enum _SafetyLine
{
	SF_Line1	= 0,
	SF_Line2	= 1,
	SF_Line3	= 2,
	SF_Line4	= 3,
	SF_Line5	= 4,
}SafetyLine;

// Functions
void LL_ToggleBoardLED();
void LL_SelfTestNum(SafetyLine Line, bool State);
void LL_LampGreen(bool State);
void LL_LampRed(bool State);
void LL_ToggleSwitchLamp(SwitchLamp Lamp, bool State);
bool LL_ReadSafetyLine(SafetyLine Line);
float LL_MEASURE_VoltageX(Int16U ADC1Channel);

#endif //__LOWLEVEL_H

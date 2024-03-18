#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "stdinc.h"

// Types
//
typedef enum _SwitchLamp
{
	Lamp_OptBarierSwitch	= 0,
	Lamp_DoorSwitch			= 1,
	Lamp_InputSwitch		= 2,
}SwitchLamp;

typedef enum _SafetyLine
{
	SF_OptBarierSwitch		= 0,
	SF_DoorSwitch			= 1,
	SF_InputSwitch			= 2,
	SF_Out1					= 3,
	SF_Out2					= 4,
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

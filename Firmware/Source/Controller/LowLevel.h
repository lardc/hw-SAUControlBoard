#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
//
#include "stdinc.h"
#include "SelfTest.h"

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

typedef enum _ColorLamp
{
	SwitchedOff	= 0,
	Green		= 1,
	Red			= 2
}ColorLamp;

// Functions
void LL_ToggleBoardLED();
void LL_SelfTestNum(DeviceSelfTestStage Stage, bool State);
void LL_StatusLamp(ColorLamp Color);
void LL_ToggleSwitchLamp(SwitchLamp Lamp, bool State);
bool LL_ReadSafetyLine(SafetyLine Line);
float LL_MEASURE_OutputVoltage(Int16U ADC1Channel);
bool LL_ReadTemperatureFlag();

#endif //__LOWLEVEL_H

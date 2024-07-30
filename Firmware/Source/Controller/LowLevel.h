#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
//
#include "stdinc.h"
#include "SelfTest.h"

// Types
//
typedef enum _LineID
{
	LID_OptBarier	= 0,
	LID_Door		= 1,
	LID_Input		= 2,
	LID_Out1		= 3,
	LID_Out2		= 4,
} LineID;

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
void LL_ToggleSwitchLamp(LineID Lamp, bool State);
bool LL_ReadSafetyLine(LineID Line);
float LL_MEASURE_OutputVoltage(Int16U ADC1Channel);
bool LL_ReadTemperatureFlag();
void LL_SwitchInputRelays(bool State);

#endif //__LOWLEVEL_H

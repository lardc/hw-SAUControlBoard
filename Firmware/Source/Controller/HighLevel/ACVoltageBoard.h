#ifndef __AC_VOLTAGE_BOARD_H
#define __AC_VOLTAGE_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "ACVoltageBoardDictionary.h"

// Definitions
#define ACV_EMULATION_RES_VOLTAGE			912
#define ACV_EMULATION_RES_CURRENT			9123

// Types
typedef struct __ACVoltageBoardObject
{
	ACV_OutputLine OutputLine;
	VIPair Setpoint;
	VIPair Result;
} ACVoltageBoardObject, *pACVoltageBoardObject;

// Functions
//
ExecutionResult ACV_Execute(NodeName Name);
ExecutionResult ACV_ReadResult(NodeName Name);
ExecutionResult ACV_Stop(NodeName Name);
ExecutionResult ACV_IsVoltageReady(NodeName Name, bool *VoltageReady);

#endif // __AC_VOLTAGE_BOARD_H

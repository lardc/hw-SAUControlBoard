#ifndef __DC_VOLTAGE_BOARD_H
#define __DC_VOLTAGE_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "DCVoltageBoardDictionary.h"

// Definitions
#define DCV_EMULATION_RES_VOLTAGE			567
#define DCV_EMULATION_RES_CURRENT			5678

// Types
typedef struct __DCVoltageBoardObject
{
	DCV_OutputLine OutputLine;
	DCV_OutputType OutputType;
	DCV_OutputMode OutputMode;
	uint16_t PulseLength;
	VIPair Setpoint;
	VIPair Result;
} DCVoltageBoardObject, *pDCVoltageBoardObject;

// Functions
//
ExecutionResult DCV_Execute(NodeName Name);
ExecutionResult DCV_ReadResult(NodeName Name);
ExecutionResult DCV_Stop(NodeName Name);
ExecutionResult DCV_IsVoltageReady(NodeName Name, bool *VoltageReady);

#endif // __DC_VOLTAGE_BOARD_H

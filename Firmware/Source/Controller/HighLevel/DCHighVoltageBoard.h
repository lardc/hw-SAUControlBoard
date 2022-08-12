#ifndef __DC_HIGH_VOLTAGE_BOARD_H
#define __DC_HIGH_VOLTAGE_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "DCHighVoltageBoardDictionary.h"

// Definitions
#define DCHV_EMULATION_RES_VOLTAGE			654
#define DCHV_EMULATION_RES_CURRENT			6543

// Types
typedef struct __DCHVoltageBoardObject
{
	VIPair Setpoint;
	VIPair Result;
} DCHVoltageBoardObject, *pDCHVoltageBoardObject;

// Functions
//
ExecutionResult DCHV_Execute();
ExecutionResult DCHV_ExecuteNext();
ExecutionResult DCHV_ReadResult();
ExecutionResult DCHV_Stop();

#endif // __DC_HIGH_VOLTAGE_BOARD_H

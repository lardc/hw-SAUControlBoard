#ifndef __CURRENT_BOARD_H
#define __CURRENT_BOARD_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "CurrentBoardDictionary.h"

// Definitions
#define CURR_EMULATION_RES_VOLTAGE			123
#define CURR_EMULATION_RES_CURRENT			1234

// Types
typedef struct __CurrentBoardObject
{
	VIPair Setpoint;
	VIPair Result;
	bool ResistanceMode;
} CurrentBoardObject, *pCurrentBoardObject;

// Functions
//
ExecutionResult CURR_Execute();
ExecutionResult CURR_ReadResult();
ExecutionResult CURR_Stop();
ExecutionResult CURR_AfterPulseReady(bool *Ready);

#endif // __CURRENT_BOARD_H

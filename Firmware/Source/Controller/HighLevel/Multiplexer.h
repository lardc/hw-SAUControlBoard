#ifndef __MULTIPLEXER_H
#define __MULTIPLEXER_H

// Include
#include "stdinc.h"
#include "Common.h"
#include "MultiplexerDictionary.h"

// Types
typedef struct __MuxObject
{
	MeasurementType MeasureType;
	DL_Case Case;
	MUX_Position Position;
	InputType InputType;
	LeakageType LeakageType;
	MUX_Control InputCommutation;
	MUX_Polarity OutputCommutationPolarity;
	bool SafetyMute;
} MuxObject, *pMuxObject;

// Functions
//
ExecutionResult MUX_Connect();
ExecutionResult MUX_ConnectFast();
ExecutionResult MUX_Disconnect();
ExecutionResult MUX_ReadStartButton(bool *Start);

#endif // __MULTIPLEXER_H

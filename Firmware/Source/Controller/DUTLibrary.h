#ifndef __DUT_LIB_H
#define __DUT_LIB_H

#include "stdinc.h"

// Types
typedef enum __DL_Case
{
	A1 = 1,
	I1 = 2,
	I6 = 3,
	B1 = 4,
	B2 = 5,
	B3 = 6,
	B5 = 7,
	V1 = 8,
	V2 = 9,
	V108 = 10,
	L1 = 11,
	L2 = 12,
	D1 = 13,
	D2 = 14,
	D192 = 15,
	V104 = 16,
	I12 = 17,
	T1 = 18,
	E2k = 19,
	A6 = 20,
	B1a = 21
} DL_Case;

typedef enum __DL_InputPositions
{
	OneInput = 0,
	TwoInputs = 1
} DL_InputPositions;

typedef enum __DL_OutputPositions
{
	OneOutput = 0,
	TwoOutputs = 1,
	ThreeOutputs = 2
} DL_OutputPositions;

typedef enum __DL_AuxPowerSupply
{
	NoSupply = 0,
	SingleDCSupply = 1,
	DoubleDCSupply = 2
} DL_AuxPowerSupply, *pDL_AuxPowerSupply;

typedef struct __DL_DUTConfiguration
{
	DL_Case Case;
	DL_InputPositions InputPositionsNum;
	DL_OutputPositions OutputPositionsNum;
	DL_AuxPowerSupply AuxPowerSupply;
} DL_DUTConfiguration, *pDL_DUTConfiguration;

// Functions
const DL_DUTConfiguration* DUTLIB_ExtractConfiguration(DL_Case Case);

#endif // __DUT_LIB_H

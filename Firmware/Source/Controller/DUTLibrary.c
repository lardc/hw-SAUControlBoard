// Header
#include "DUTLibrary.h"

// Variables
//
static const DL_DUTConfiguration DUTConfigStorage[] =
{
		{A1,	OneInput, OneOutput, NoSupply},
		{I1,	OneInput, OneOutput, NoSupply},
		{I6,	OneInput, OneOutput, SingleDCSupply},
		{B1,	OneInput, OneOutput, NoSupply},
		{B2,	OneInput, OneOutput, NoSupply},
		// B3
		{B5,	OneInput, OneOutput, SingleDCSupply},
		{V1,	OneInput, OneOutput, NoSupply},
		{V2,	OneInput, OneOutput, NoSupply},
		{V108,	OneInput, OneOutput, DoubleDCSupply},
		{L1,	OneInput, OneOutput, NoSupply},
		{L2,	TwoInputs, TwoOutputs, NoSupply},
		{D1,	OneInput, ThreeOutputs, NoSupply},
		{D2,	OneInput, ThreeOutputs, NoSupply},
		{D192,	OneInput, ThreeOutputs, NoSupply},
		{V104,	OneInput, OneOutput, NoSupply},
		{I12,	OneInput, TwoOutputs, NoSupply},
		{T1,	OneInput, OneOutput, NoSupply},
		{E2k,	TwoInputs, TwoOutputs, NoSupply},
		{A6,	OneInput, TwoOutputs, NoSupply},
		{B1a,	OneInput, OneOutput, NoSupply}
};

// Forward functions
uint16_t DUTLIB_GetStorageSize();

// Functions
uint16_t DUTLIB_GetStorageSize()
{
	return sizeof(DUTConfigStorage) / sizeof(DUTConfigStorage[0]);
}
//------------------------------------------------

const DL_DUTConfiguration* DUTLIB_ExtractConfiguration(DL_Case Case)
{
	uint8_t DevNum = DUTLIB_GetStorageSize();
	for(uint8_t i = 0; i < DevNum; ++i)
	{
		if(Case == DUTConfigStorage[i].Case)
			return &DUTConfigStorage[i];
	}
	return NULL;
}
//------------------------------------------------

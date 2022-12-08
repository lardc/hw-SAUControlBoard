// Header
#include "DebugActions.h"

// Include
//
#include "LowLevel.h"
#include "Board.h"
#include "DataTable.h"
#include "Delay.h"

// Functions
//
//-----------------------------------------------

void DBGACT_INT_FAN()
{
	if(DataTable[REG_INT_FAN])
		LL_INT_FAN(TRUE);
	else
		LL_INT_FAN(FALSE);
}

//-----------------------------------------------

void DBGACT_GREEN_LED()
{
	LL_ExternalLampGREEN(TRUE);
	DELAY_US(500000);
	LL_ExternalLampGREEN(FALSE);
}

//-----------------------------------------------

void DBGACT_RED_LED()
{
	LL_ExternalLampRED(TRUE);
	DELAY_US(500000);
	LL_ExternalLampRED(FALSE);
}

//-----------------------------------------------

void DBGACT_PC_SWITCH()
{
	LL_PC_SWITCH();
}

//-----------------------------------------------





// Header
#include "DebugActions.h"

// Include
#include "LowLevel.h"
#include "Board.h"
#include "DataTable.h"
#include "Delay.h"
#include "Global.h"

// Functions
void DBGACT_Relay()
{
	LL_SelfTestNum(DataTable[REG_DBG], true);
	DELAY_MS(1000);
	LL_SelfTestNum(DataTable[REG_DBG], false);
}
//-----------------------------------------------

void DBGACT_Switch()
{
	for(int i =  0; i < SAFETY_SWITCH_NUM; i++)
	{
		if(!LL_ReadSafetyLine(i))
			LL_ToggleSwitchLamp(i, true);
		else
			LL_ToggleSwitchLamp(i, false);
	}
}
//-----------------------------------------------

void DBGACT_LampGreen()
{
	LL_LampGreen(true);
	DELAY_US(500000);
	LL_LampGreen(false);
}
//-----------------------------------------------

void DBGACT_LampRed()
{
	LL_LampRed(true);
	DELAY_US(500000);
	LL_LampRed(false);
}
//-----------------------------------------------

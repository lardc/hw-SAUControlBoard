// Header
#include "DebugActions.h"

// Include
#include "LowLevel.h"
#include "Board.h"
#include "DataTable.h"
#include "Delay.h"
#include "Global.h"

// Functions
//
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch(ActionID)
	{
		case ACT_DIAG_SELFTEST_RELAY:
			DBGACT_Relay();
			break;

		case ACT_DIAG_SWITCH:
			DBGACT_Switch();
			break;

		case ACT_DIAG_GREEN_LED:
			DBGACT_LampGreen();
			break;

		case ACT_DIAG_RED_LED:
			DBGACT_LampRed();
			break;

		default:
			return false;
			break;
	}

	return true;
}
//-----------------------------------------------

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
	LL_StatusLamp(Green);
}
//-----------------------------------------------

void DBGACT_LampRed()
{
	LL_StatusLamp(Red);
}
//-----------------------------------------------

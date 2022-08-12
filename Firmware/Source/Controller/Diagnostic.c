// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "LowLevel.h"
#include "Controller.h"
#include "Delay.h"

// Forward functions
void DBGACT_BlinkExtLed();
void DBGACT_GenerateImpulseToExtLineSync1();
void DBGACT_GenerateImpulseToExtLineSync2();
void DBGACT_GenerateImpulseToIntLineSync1();
void DBGACT_GenerateImpulseToIntLineSync2();
bool DBGACT_ReadStateIntLineSync1();
bool DBGACT_ReadStateIntLineSync2();

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch (ActionID)
	{
		case ACT_DBG_PULSE_EXT_SYNC1:
			{
				DBGACT_GenerateImpulseToExtLineSync1();
			}
			break;

		case ACT_DBG_PULSE_EXT_SYNC2:
			{
				DBGACT_GenerateImpulseToExtLineSync2();
			}
			break;

		case ACT_DBG_PULSE_INT_SYNC1:
			{
				DBGACT_GenerateImpulseToIntLineSync1();
			}
			break;

		case ACT_DBG_PULSE_INT_SYNC2:
			{
				DBGACT_GenerateImpulseToIntLineSync2();
			}
			break;

		case ACT_DBG_IS_STATE_INT_SYNC_1:
			{
				DataTable[REG_DBG_INT_SYNC1] = DBGACT_ReadStateIntLineSync1();
			}
			break;

		case ACT_DBG_IS_STATE_INT_SYNC_2:
			{
				DataTable[REG_DBG_INT_SYNC2] = DBGACT_ReadStateIntLineSync2();
			}
			break;

		case ACT_DBG_PULSE_EXT_LED:
			{
				DBGACT_BlinkExtLed();
			}
			break;

		case ACT_FAN_ON:
			{
				LL_SetStateFan(true);
				DataTable[REG_FAN_STATE] = true;
			}
			break;

		case ACT_FAN_OFF:
			{
				LL_SetStateFan(false);
				DataTable[REG_FAN_STATE] = false;
			}
			break;

		default:
			return false;
	}

	return true;
}
// ----------------------------------------

void DBGACT_BlinkExtLed()
{
	LL_SetStateExtLed(true);
	DELAY_MS(1000);
	LL_SetStateExtLed(false);
}
//-----------------------------

void DBGACT_GenerateImpulseToExtLineSync1()
{
	LL_SetStateExtLineSync1(true);
	DELAY_MS(100);
	LL_SetStateExtLineSync1(false);
}
//-----------------------------

void DBGACT_GenerateImpulseToExtLineSync2()
{
	LL_SetStateExtLineSync2(true);
	DELAY_MS(100);
	LL_SetStateExtLineSync2(false);
}
//-----------------------------

void DBGACT_GenerateImpulseToIntLineSync1()
{
	LL_SetStateIntLineSync1(true);
	DELAY_MS(100);
	LL_SetStateIntLineSync1(false);
}
//-----------------------------

void DBGACT_GenerateImpulseToIntLineSync2()
{
	LL_SetStateIntLineSync2(true);
	DELAY_MS(100);
	LL_SetStateIntLineSync2(false);
}
//-----------------------------

bool DBGACT_ReadStateIntLineSync1()
{
	return LL_GetStateIntLineSync1();
}
//-----------------------------

bool DBGACT_ReadStateIntLineSync2()
{
	return LL_GetStateIntLineSync2();
}
//-----------------------------

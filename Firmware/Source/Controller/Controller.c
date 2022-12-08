// ----------------------------------------
// Controller logic
// ----------------------------------------

// Header
#include "Controller.h"
//
// Includes
#include "SysConfig.h"
#include "Board.h"
#include "DeviceObjectDictionary.h"
#include "DataTable.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "BCCIxParams.h"
#include "DebugActions.h"
#include "LowLevel.h"

// Variables
volatile Int64U CONTROL_TimeCounter = 0;
Boolean CycleActive = false;

// Forward functions
Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_Idle();
void CONTROL_UpdateWatchDog();
void CONTROL_Init();



// Functions
void CONTROL_Init()
{
	// Init data table
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, CAN_MASTER_NID);
	
	// Device profile initialization
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_ResetControlSection();
	DataTable[REG_MME_CODE] = MME_CODE;
}

// ----------------------------------------

void CONTROL_INT_FAN()
{
	if (DataTable[REG_INT_FAN] == 1)
	{
		LL_INT_FAN(TRUE);
	}
	else
	{
		LL_INT_FAN(FALSE);
	}
}

// ----------------------------------------

void CONTROL_GREEN_LED()
{
	if (DataTable[REG_LAMP_GREEN] == 1)
	{
		LL_ExternalLampGREEN(TRUE);
	}
	else
	{
		LL_ExternalLampGREEN(FALSE);
	}
}

// ----------------------------------------

void CONTROL_RED_LED()
{
	if (DataTable[REG_LAMP_RED] == 1)
	{
		LL_ExternalLampRED(TRUE);
	}
	else
	{
		LL_ExternalLampRED(FALSE);
	}
}

// ----------------------------------------

void CONTROL_EXT_BUTTON()
{
	if (LL_ExternalButton())
	{
		DataTable[REG_EXT_BUTTON] = 0;
	}
	else
	{
		DataTable[REG_EXT_BUTTON] = 1;
	}
}

// ----------------------------------------

void CONTROL_Idle()
{
	// Управление зеленым индикатором
	CONTROL_GREEN_LED();
	// Управление красным индикатором
	CONTROL_RED_LED();
	// Управление вентилятором
	CONTROL_INT_FAN();
	// Состояние внешней кнопки
	CONTROL_EXT_BUTTON();

	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
}

// ----------------------------------------

Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_DIAG_INT_FAN:
			DBGACT_INT_FAN();
		break;

		case ACT_DIAG_GREEN_LED:
			DBGACT_GREEN_LED();
		break;

		case ACT_DIAG_RED_LED:
			DBGACT_RED_LED();
		break;

		case ACT_DIAG_PC_SWITCH:
			DBGACT_PC_SWITCH();
		break;
	default:
		return false;
	}

	return true;
}
// ----------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
// ----------------------------------------

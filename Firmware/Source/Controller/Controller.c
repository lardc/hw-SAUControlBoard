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
#include "Diagnostic.h"
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

void CONTROL_Idle()
{


	// Управление зеленым индикатором
	CONTROL_GREEN_LED();
	// Управление красным индикатором
	CONTROL_RED_LED();
	// Управление вентилятором
	CONTROL_INT_FAN();

	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
}

// ----------------------------------------

Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{

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

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
#include "SelfTest.h"

// Variables
volatile Int64U CONTROL_TimeCounter = 0;
Boolean CycleActive = false;
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSelfTestState CONTROL_SubState = STS_None;

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

void CONTROL_Idle()
{
	SELFTEST_Process();
	CONTROL_SafetyInputs();

	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
}
// ----------------------------------------

Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch(ActionID)
	{
		case ACT_SET_ACTIVE:
			if(CONTROL_State == DS_None)
			{
				LL_LampGreen(false);
				LL_LampRed(true);

				CONTROL_SetDeviceState(DS_SafetyActive);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_SET_INACTIVE:
			if(CONTROL_State == DS_SafetyActive || CONTROL_State == DS_SafetyTrig)
			{
				LL_LampGreen(true);
				LL_LampRed(false);

				CONTROL_SetDeviceState(DS_None);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, UserError);
	}

	return true;
}
// ----------------------------------------

void CONTROL_SafetySwitchCheck()
{
	static Int64U BlinkPeriodCounter = 0;

	if(CONTROL_TimeCounter >= BlinkPeriodCounter)
	{
		BlinkPeriodCounter = CONTROL_TimeCounter + TIME_SAFETY_SWITCH_BLINK;

		if(LL_ReadSafetyLine(SF_OptBarierSwitch))
			LL_ToggleSwitchLamp(Lamp_OptBarierSwitch, true);
		else
			LL_ToggleSwitchLamp(Lamp_OptBarierSwitch, false);

		if(LL_ReadSafetyLine(SF_DoorSwitch))
			LL_ToggleSwitchLamp(Lamp_DoorSwitch, true);
		else
			LL_ToggleSwitchLamp(Lamp_DoorSwitch, false);

		if(LL_ReadSafetyLine(SF_InputSwitch))
			LL_ToggleSwitchLamp(Lamp_InputSwitch, true);
		else
			LL_ToggleSwitchLamp(Lamp_InputSwitch, false);
	}
}

void CONTROL_SafetyInputs()
{

}
// ----------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//------------------------------------------

void CONTROL_SetDeviceSubState(DeviceSelfTestState NewSubState)
{
	CONTROL_SubState = NewSubState;
	DataTable[REG_SUB_STATE] = NewSubState;
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
// ----------------------------------------

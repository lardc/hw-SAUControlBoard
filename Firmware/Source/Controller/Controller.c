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
volatile DeviceState CONTROL_State = DS_InSelfTest;

// Forward functions
Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_Idle();
void CONTROL_UpdateWatchDog();
void CONTROL_Init();
void CONTROL_SafetyOutputs();
void CONTROL_SafetySwitchCheck();

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
	CONTROL_SafetyOutputs();
	CONTROL_SafetySwitchCheck();

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
				LL_StatusLamp(Red);
				CONTROL_SetDeviceState(DS_SafetyActive);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_SET_INACTIVE:
			if(CONTROL_State == DS_SafetyActive || CONTROL_State == DS_SafetyTrig)
			{
				LL_StatusLamp(Green);
				CONTROL_SetDeviceState(DS_None);
			}
			else
				*UserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_START_SELF_TEST:
			if(CONTROL_State != DS_Fault)
			{
				CONTROL_SetDeviceState(DS_InSelfTest);
				SELFTTEST_SetStage(STS_None);
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
// ----------------------------------------

void CONTROL_SafetyOutputs()
{
	if(CONTROL_State != DS_InSelfTest)
	{
		if(LL_ReadSafetyLine(SF_Out1) && LL_MEASURE_OutputVoltage(ADC1_OUTPUT1) >= OUTPUT_THRESHOLD_VOLTAGE)
			CONTROL_SwitchToFault(DF_SHORT_OUTPUT1);
		else if(LL_ReadSafetyLine(SF_Out2) && LL_MEASURE_OutputVoltage(ADC1_OUTPUT2) >= OUTPUT_THRESHOLD_VOLTAGE)
				CONTROL_SwitchToFault(DF_SHORT_OUTPUT2);
		else if((!LL_ReadSafetyLine(SF_Out1) || !LL_ReadSafetyLine(SF_Out2)) && CONTROL_State == DS_SafetyActive)
				CONTROL_SwitchToFault(DS_SafetyTrig);
	}
}
// ----------------------------------------

void CONTROL_Indication()
{
	static bool ToggleState = false;
	static Int64U BlinkCounter = 0;

	if(CONTROL_State != DS_InSelfTest)
	{
		if(CONTROL_State == DS_Fault)
		{
			if(++BlinkCounter > TIME_FAULT_LED_BLINK)
			{
				ToggleState = ~ToggleState;
				LL_StatusLamp(ToggleState);
				BlinkCounter = 0;
			}
		}
		else
			LL_StatusLamp(DataTable[REG_STATUS_INDICATION]);

		DataTable[REG_TEMPERATURE_FLAG] = LL_ReadTemperatureFlag();
	}
}
// ----------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_SetDeviceState(DS_Fault);
	DataTable[REG_FAULT_REASON] = Reason;

	if(Reason == DF_SELF_TEST)
		DataTable[REG_SELF_TEST_OP_RESULT] = OPRESULT_FAIL;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
// ----------------------------------------

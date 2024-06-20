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

// Definitions
#define OUTPUT_CHECK_DELAY			500		// мс

// Variables
volatile Int64U CONTROL_TimeCounter = 0;
Boolean CycleActive = false;
volatile DeviceState CONTROL_State = DS_None;
Int64U CONTROL_OuputCheckDelayCounter = 0;

// Forward functions
Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U UserError);
void CONTROL_Idle();
void CONTROL_UpdateWatchDog();
void CONTROL_Init();
void CONTROL_Safety();
void CONTROL_SafetySwitchCheck();
void CONTROL_OutputCheck(LineID Line);

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
	CONTROL_Safety();
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

		case ACT_CLR_FAULT:
			if(CONTROL_State == DS_Fault)
			{
				CONTROL_OuputCheckDelayCounter = OUTPUT_CHECK_DELAY;
				DataTable[REG_FAULT_REASON] = DF_NONE;

				CONTROL_SetDeviceState(DS_InSelfTest);
				SELFTTEST_SetStage(STS_None);
			}
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

		LL_ToggleSwitchLamp(LID_OptBarier, LL_ReadSafetyLine(LID_OptBarier));
		LL_ToggleSwitchLamp(LID_Door, LL_ReadSafetyLine(LID_Door));
		LL_ToggleSwitchLamp(LID_Input, LL_ReadSafetyLine(LID_Input));
	}
}
// ----------------------------------------

void CONTROL_Safety()
{
	if(CONTROL_State != DS_InSelfTest && CONTROL_State != DS_Fault)
	{
		LL_SwitchInputRelays(false);

		if((!LL_ReadSafetyLine(LID_Out1) || !LL_ReadSafetyLine(LID_Out2)) && CONTROL_State == DS_SafetyActive)
		{
			if( DataTable[REG_USE_TRIG])
				CONTROL_SetDeviceState(DS_SafetyTrig);
			else
				CONTROL_SetDeviceState(DS_SafetyActive);
		}
		if(CONTROL_TimeCounter >= CONTROL_OuputCheckDelayCounter)
		{
			CONTROL_OutputCheck(LID_Out1);
			CONTROL_OutputCheck(LID_Out2);
		}
	}
}
// ----------------------------------------

void CONTROL_OutputCheck(LineID Line)
{
	Int16U ADCChannel, Fault;

	switch(Line)
	{
		default:
		case LID_Out1:
			ADCChannel = ADC1_OUTPUT1;
			Fault = DF_SHORT_OUTPUT1;
			break;

		case LID_Out2:
			ADCChannel = ADC1_OUTPUT2;
			Fault = DF_SHORT_OUTPUT2;
			break;
	}

	if(LL_ReadSafetyLine(Line))
	{
		DELAY_MS(1);
		if(LL_ReadSafetyLine(Line) && LL_MEASURE_OutputVoltage(ADCChannel) >= OUTPUT_THRESHOLD_VOLTAGE)
		{
			LL_SwitchInputRelays(true);
			CONTROL_SwitchToFault(Fault);
		}
	}
}
// ----------------------------------------

void CONTROL_Indication()
{
	static ColorLamp ToggleState = false;
	static Int64U BlinkCounter = 0;

	if(CONTROL_State == DS_Fault)
	{
		if(++BlinkCounter > TIME_FAULT_LED_BLINK)
		{
			ToggleState = (ToggleState == SwitchedOff) ? Red : SwitchedOff;
			LL_StatusLamp(ToggleState);
			BlinkCounter = 0;
		}
	}
	else if(CONTROL_State != DS_InSelfTest)
			LL_StatusLamp(DataTable[REG_STATUS_INDICATION]);

	DataTable[REG_TEMPERATURE_FLAG] = LL_ReadTemperatureFlag();
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

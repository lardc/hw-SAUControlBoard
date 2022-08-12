// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "Global.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "Diagnostic.h"
#include "BCCIMHighLevel.h"
#include "BCCIxParams.h"
#include "Logic.h"
#include "LogicLeakage.h"
#include "LogicOnState.h"
#include "LogicControl.h"
#include "LogicInhibit.h"
#include "LogicCalibrate.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
DeviceState CONTROL_State = DS_None;
DeviceSubState CONTROL_SubState = DSS_None;
static Boolean CycleActive = false, SafetyMonitorActive = false;
static Int16U CONTROL_EPCounter, CONTROL_EPArray[XCCI_EP_SIZE];

volatile Int64U CONTROL_TimeCounter = 0;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();
void CONTROL_ResetOutputRegisters();
void CONTROL_ClearFault();
void CONTROL_HandleFanLogic();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_SLAVE_DATA};
	Int16U EPSized[EP_COUNT] = {XCCI_EP_SIZE};
	pInt16U EPCounters[EP_COUNT] = {&CONTROL_EPCounter};
	pInt16U EPDatas[EP_COUNT] = {CONTROL_EPArray};

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SLAVE_NID, CAN_MASTER_NID);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();

	// Инициализация сущностей логики верхнего уровня
	LOGIC_InitEntities();
}
//------------------------------------------

void CONTROL_ResetOutputRegisters()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	DataTable[REG_FAILED_DEV_SUB_STATE] = 0;
	DataTable[REG_CONFIG_ERR] = LCE_None;
	
	DataTable[REG_RESULT_LEAKAGE_CURRENT] = 0;
	DataTable[REG_RESULT_ON_VOLTAGE] = 0;
	DataTable[REG_RESULT_CONTROL_CURRENT] = 0;
	DataTable[REG_RESULT_CONTROL_VOLTAGE] = 0;
	DataTable[REG_RESULT_INHIBIT_VOLTAGE] = 0;
	DataTable[REG_RESULT_AUX_CURRENT1] = 0;
	DataTable[REG_RESULT_AUX_CURRENT2] = 0;
	DataTable[REG_RESULT_OUTPUT_RES] = 0;

	DataTable[REG_RESULT_LEAKAGE_VOLTAGE] = 0;
	DataTable[REG_RESULT_LEAKAGE_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_ON_CURRENT] = 0;
	DataTable[REG_RESULT_ON_CURRENT_32] = 0;

	DataTable[REG_RESULT_LEAKAGE_CURRENT_32] = 0;
	DataTable[REG_RESULT_ON_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_CONTROL_CURRENT_32] = 0;
	DataTable[REG_RESULT_CONTROL_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_INHIBIT_VOLTAGE_32] = 0;
	DataTable[REG_RESULT_AUX_CURRENT1_32] = 0;
	DataTable[REG_RESULT_AUX_CURRENT2_32] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	CONTROL_ResetOutputRegisters();
	CONTROL_SetDeviceState(DS_None, DSS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
	CONTROL_HandleFanLogic();

	// Обработка логики общих операция
	LOGIC_HandleStateUpdate();
	LOGIC_HandlePowerOn();
	LOGIC_HandlePowerOff();
	LOGIC_HandleFaultAndStop();

	// Обработка логики измерений
	LEAK_HandleMeasurement();
	ONSTATE_HandleMeasurement();
	CTRL_HandleMeasurement();
	INHIBIT_HandleMeasurement();

	// Обработка логики калибровки
	CALIBRATE_HandleMeasurement();
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			{
				if(CONTROL_State == DS_None)
				{
					COMM_UpdateEmulationSettings();
					CONTROL_SetDeviceState(DS_InProcess, DSS_PowerEnable);
				}
				else if(CONTROL_State != DS_Ready && CONTROL_State != DS_SafetyTrig)
					*pUserError = ERR_DEVICE_NOT_READY;
			}
			break;
			
		case ACT_DISABLE_POWER:
			{
				if(CONTROL_State == DS_Ready || CONTROL_State == DS_SafetyTrig)
				{
					CONTROL_ResetOutputRegisters();
					CONTROL_SetDeviceState(DS_None, DSS_PowerOff);
				}
				else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_SAFETY_SET_ACTIVE:
			{
				if(CONTROL_State == DS_Ready || CONTROL_State == DS_SafetyTrig)
				{
					SafetyMonitorActive = true;
					CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_SAFETY_SET_INACTIVE:
			{
				if(CONTROL_State == DS_Ready || CONTROL_State == DS_SafetyTrig)
				{
					SafetyMonitorActive = false;
					CONTROL_SetDeviceState(DS_Ready, DSS_None);
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_CALIBRATE:
		case ACT_START_TEST:
			{
				if(CONTROL_State == DS_Ready || CONTROL_State == DS_SafetyTrig)
				{
					LL_SetStateExtLed(true);
					LL_SetStateExtLineSync1(true);

					COMM_UpdateEmulationSettings();
					CONTROL_ResetOutputRegisters();

					LogicConfigError err = LOGIC_PrepareMeasurement(ActionID == ACT_CALIBRATE);
					DataTable[REG_CONFIG_ERR] = err;

					if(err != LCE_None)
					{
						LL_SetStateExtLed(false);
						LL_SetStateExtLineSync1(false);

						*pUserError = ERR_BAD_CONFIG;
					}
				}
				else
					*pUserError = ERR_OPERATION_BLOCKED;
			}
			break;

		case ACT_STOP_TEST:
			{
				if(CONTROL_State == DS_InProcess && CONTROL_SubState < DSS_InterruptableStatesEnd)
				{
					CONTROL_SetDeviceState(DS_InProcess, DSS_Stop_Request);

					DataTable[REG_WARNING] = WARNING_COMMAND_STOP;
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
				}
			}
			break;

		case ACT_FAULT_CLEAR:
			{
				if(CONTROL_State == DS_Fault)
				{
					COMM_UpdateEmulationSettings();
					COMM_SlavesClearFault();
					CONTROL_ClearFault();
				}
			}
			break;

		case ACT_WARNING_CLEAR:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		case ACT_SAFETY_CLEAR:
			if(CONTROL_State == DS_SafetyTrig)
				CONTROL_SetDeviceState(DS_Ready, DSS_None);
			break;

		case ACT_DIAG_READ_REG:
			{
				uint16_t val;
				bool ret = BHL_ReadRegister(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], &val);
				if(ret)
				{
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
					DataTable[REG_DIAG_OUT_2] = val;
				}
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_WRITE_REG:
			{
				bool ret = BHL_WriteRegister(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], DataTable[REG_DIAG_IN_2]);
				if(ret)
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_CALL:
			{
				bool ret = BHL_Call(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1]);
				if(ret)
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		case ACT_DIAG_READ_EP:
			{
				DEVPROFILE_ResetEPReadState();
				DEVPROFILE_ResetScopes(0);

				bool ret = BHL_ReadArray16(DataTable[REG_DIAG_NID], DataTable[REG_DIAG_IN_1], CONTROL_EPArray, XCCI_EP_SIZE, &CONTROL_EPCounter);
				if (ret)
				{
					DataTable[REG_DIAG_OUT_1] = ERR_NO_ERROR;
					DataTable[REG_DIAG_OUT_2] = CONTROL_EPCounter;
				}
				else
				{
					BHLError err = BHL_GetError();
					DataTable[REG_DIAG_OUT_1] = err.ErrorCode;
				}
			}
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
	}

	return true;
}
//-----------------------------------------------

void CONTROL_SwitchToFault(ExecutionResult Result, Int16U Group)
{
	if(Result != ER_NoError && CONTROL_SubState < DSS_InterruptableStatesEnd)
	{
		if(Result == ER_InterfaceError)
		{
			BHLError Error = BHL_GetError();
			DataTable[REG_BHL_ERROR_CODE] = Error.ErrorCode;
			DataTable[REG_BHL_DEVICE] = Error.Device;
			DataTable[REG_BHL_FUNCTION] = Error.Func;
			DataTable[REG_BHL_EXT_DATA] = Error.ExtData;
			DataTable[REG_BHL_DETAILS] = Error.Details;

			if(Error.ErrorCode == ERR_TIMEOUT)
				COMM_ForceEmulationByNodeID(Error.Device);
		}

		DataTable[REG_FAILED_DEV_SUB_STATE] = CONTROL_SubState;
		DataTable[REG_FAULT_REASON] = Result + Group;

		CONTROL_SetDeviceState(DS_InProcess, DSS_Fault_Request);
	}
}
//------------------------------------------

void CONTROL_ClearFault()
{
	CONTROL_SetDeviceState(DS_None, DSS_None);
	DataTable[REG_FAULT_REASON] = 0;
	DataTable[REG_FAILED_DEV_SUB_STATE] = 0;

	BHL_ResetError();
	DataTable[REG_BHL_ERROR_CODE] = 0;
	DataTable[REG_BHL_DEVICE] = 0;
	DataTable[REG_BHL_FUNCTION] = 0;
	DataTable[REG_BHL_EXT_DATA] = 0;
	DataTable[REG_BHL_DETAILS] = 0;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_State = NewState;
	DataTable[REG_DEV_STATE] = NewState;

	CONTROL_SubState = NewSubState;
	DataTable[REG_DEV_SUB_STATE] = NewSubState;

	if(CONTROL_SubState == DSS_None)
	{
		LL_SetStateExtLed(false);
		LL_SetStateExtLineSync1(false);
	}
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------

void CONTROL_HandleFanLogic()
{
	static Int64U FANOnNextTime = 0;
	static Int64U FANOffTime = 0;

	if(DataTable[REG_DIAG_FAN_MUTE])
	{
		LL_SetStateFan(false);
	}
	else
	{
		if(CONTROL_State == DS_InProcess || CONTROL_TimeCounter > FANOnNextTime || FANOnNextTime == 0 || FANOffTime == 0)
		{
			FANOnNextTime = CONTROL_TimeCounter + (Int32U)DataTable[REG_FAN_OPERATE_PERIOD] * 1000;
			FANOffTime = CONTROL_TimeCounter + (Int32U)DataTable[REG_FAN_OPERATE_MIN_TIME] * 1000;
			LL_SetStateFan(true);
		}
		else if(CONTROL_TimeCounter > FANOffTime)
			LL_SetStateFan(false);
	}
}
// ----------------------------------------

bool CONTROL_IsSafetyActive()
{
	return SafetyMonitorActive;
}
// ----------------------------------------

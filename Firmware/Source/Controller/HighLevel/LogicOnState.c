// Header
#include "LogicOnState.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "Common.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"
#include "CurrentBoard.h"

// Variables
static const VIPair ZeroVI = {0, 0};
static VIPair Control, Current, PS1, PS2;

// Functions
void ONSTATE_HandleMeasurement()
{
	static Int64U Timeout = 0;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();
		LOGIC_Wrapper_SafetyMonitor();

		switch(CONTROL_SubState)
		{
			case DSS_OnVoltage_StartTest:
				Control = Current = PS1 = PS2 = ZeroVI;
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_OnVoltage_Commutate);
				break;

			case DSS_OnVoltage_Commutate:
				LOGIC_Wrapper_Commutate(DSS_OnVoltage_WaitCommutation, DSS_OnVoltage_SaveResult, &Problem);
				break;

			case DSS_OnVoltage_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_InitAfterPulseWait);
				break;

			case DSS_OnVoltage_InitAfterPulseWait:
				LOGIC_Wrapper_CurrentAfterPulseSetTimeout(DSS_OnVoltage_AfterPulseWait, &Timeout);
				break;

			case DSS_OnVoltage_AfterPulseWait:
				LOGIC_Wrapper_WaitCurrentAfterPulse(DSS_OnVoltage_StartPowerSupply, DSS_OnVoltage_UnCommutate, Timeout, &Problem);
				break;

			case DSS_OnVoltage_StartPowerSupply:
				LOGIC_Wrapper_StartPowerSupply(DSS_OnVoltage_WaitStartPowerSupply, DSS_OnVoltage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_WaitStartPowerSupply:
				LOGIC_Wrapper_IsPowerSupplyOutputReady(DSS_OnVoltage_StartControl, DSS_OnVoltage_StopPowerSupply,
						Timeout, &Problem);
				break;

			case DSS_OnVoltage_StartControl:
				LOGIC_Wrapper_StartControl(DSS_OnVoltage_WaitControlReady, DSS_OnVoltage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_WaitControlReady:
				LOGIC_Wrapper_IsControlOutputReady(DSS_OnVoltage_SetControlDelay, DSS_OnVoltage_StopControl,
						Timeout, &Problem, false);
				break;

			case DSS_OnVoltage_SetControlDelay:
				LOGIC_Wrapper_ControlSetDelay(DSS_OnVoltage_WaitControlDelay, DSS_OnVoltage_PulseCurrent, &Timeout);
				break;

			case DSS_OnVoltage_WaitControlDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_OnVoltage_CheckReadyAfterDelay, Timeout);
				break;

			case DSS_OnVoltage_CheckReadyAfterDelay:
				LOGIC_Wrapper_IsControlOutputReady(DSS_OnVoltage_PulseCurrent, DSS_OnVoltage_StopControl,
						0, &Problem, false);
				break;

			case DSS_OnVoltage_PulseCurrent:
				LOGIC_Wrapper_PulseCurrent(DSS_OnVoltage_WaitCurrentReady, DSS_OnVoltage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_OnVoltage_WaitCurrentReady:
				LOGIC_Wrapper_WaitCurrentFinished(DSS_OnVoltage_StopControl, Timeout);
				break;

			case DSS_OnVoltage_StopControl:
				LOGIC_Wrapper_StopControl(DSS_OnVoltage_WaitStopControl);
				break;

			case DSS_OnVoltage_WaitStopControl:
				LOGIC_Wrapper_IsControlNodeReady(DSS_OnVoltage_StopPowerSupply);
				break;

			case DSS_OnVoltage_StopPowerSupply:
				LOGIC_Wrapper_StopPowerSupply(DSS_OnVoltage_WaitStopPowerSupply);
				break;

			case DSS_OnVoltage_WaitStopPowerSupply:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_UnCommutate);
				break;

			case DSS_OnVoltage_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_OnVoltage_WaitUnCommutate);
				break;

			case DSS_OnVoltage_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_OnVoltage_ReadResultPS1);
				break;

			case DSS_OnVoltage_ReadResultPS1:
				LOGIC_Wrapper_PowerSupply1ReadResult(DSS_OnVoltage_ReadResultPS2, &PS1, &Problem);
				break;

			case DSS_OnVoltage_ReadResultPS2:
				LOGIC_Wrapper_PowerSupply2ReadResult(DSS_OnVoltage_ReadResultControl, &PS2, &Problem);
				break;

			case DSS_OnVoltage_ReadResultControl:
				LOGIC_Wrapper_ControlReadResult(DSS_OnVoltage_ReadResultCurrent, &Control, &Problem);
				break;

			case DSS_OnVoltage_ReadResultCurrent:
				LOGIC_Wrapper_CurrentReadResult(DSS_OnVoltage_SaveResult, &Current, &Problem);
				break;

			case DSS_OnVoltage_SaveResult:
				if(Problem == PROBLEM_NONE)
				{
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
					LOGIC_Wrapper_PowerSupply1SaveResult(PS1);
					LOGIC_Wrapper_PowerSupply2SaveResult(PS2);
					LOGIC_Wrapper_ControlSaveResult(Control);
					LOGIC_Wrapper_CurrentSaveResult(Current);
				}
				else
				{
					DataTable[REG_OP_RESULT] = OPRESULT_FAIL;
					DataTable[REG_PROBLEM] = Problem;
				}
				CONTROL_SetDeviceState(DS_Ready, DSS_None);
				break;

			default:
				break;
		}
	}
}
//-----------------------------

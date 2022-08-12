// Header
#include "LogicLeakage.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"

// Variables
static const VIPair ZeroVI = {0, 0};
static VIPair Control, Leakage, PS1, PS2;

// Functions
void LEAK_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();
		LOGIC_Wrapper_SafetyMonitor();

		switch(CONTROL_SubState)
		{
			case DSS_Leakage_StartTest:
				Control = Leakage = PS1 = PS2 = ZeroVI;
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Leakage_Commutate);
				break;

			case DSS_Leakage_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Leakage_WaitCommutation, DSS_Leakage_SaveResult, &Problem);
				break;

			case DSS_Leakage_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_StartPowerSupply);
				break;

			case DSS_Leakage_StartPowerSupply:
				LOGIC_Wrapper_StartPowerSupply(DSS_Leakage_WaitStartPowerSupply, DSS_Leakage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitStartPowerSupply:
				LOGIC_Wrapper_IsPowerSupplyOutputReady(DSS_Leakage_StartControl, DSS_Leakage_StopPowerSupply,
						Timeout, &Problem);
				break;

			case DSS_Leakage_StartControl:
				LOGIC_Wrapper_StartControl(DSS_Leakage_WaitControlReady, DSS_Leakage_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitControlReady:
				LOGIC_Wrapper_IsControlOutputReady(DSS_Leakage_SetControlDelay, DSS_Leakage_StopControl,
						Timeout, &Problem, false);
				break;

			case DSS_Leakage_SetControlDelay:
				LOGIC_Wrapper_ControlSetDelay(DSS_Leakage_WaitControlDelay, DSS_Leakage_StartOutVoltage, &Timeout);
				break;

			case DSS_Leakage_WaitControlDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_Leakage_CheckReadyAfterDelay, Timeout);
				break;

			case DSS_Leakage_CheckReadyAfterDelay:
				LOGIC_Wrapper_IsControlOutputReady(DSS_Leakage_StartOutVoltage, DSS_Leakage_StopControl,
						0, &Problem, false);
				break;

			case DSS_Leakage_StartOutVoltage:
				LOGIC_Wrapper_StartLeakage(DSS_Leakage_WaitReadyForNextStart, DSS_Leakage_StopControl,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitReadyForNextStart:
				LOGIC_Wrapper_IsLeakageReadyForNext(DSS_Leakage_MakeFastCommutation, DSS_Leakage_StopOutVoltage,
						Timeout, &Problem);
				break;

			case DSS_Leakage_MakeFastCommutation:
				LOGIC_Wrapper_CommutateFast(DSS_Leakage_WaitFastCommutationReady, DSS_Leakage_StopOutVoltage, &Problem);
				break;

			case DSS_Leakage_WaitFastCommutationReady:
				LOGIC_Wrapper_IsCommutationNodeReady(DSS_Leakage_NextStartOutVoltage);
				break;

			case DSS_Leakage_NextStartOutVoltage:
				LOGIC_Wrapper_StartLeakageNext(DSS_Leakage_WaitOutVoltageReady, DSS_Leakage_StopOutVoltage,
						&Timeout, &Problem);
				break;

			case DSS_Leakage_WaitOutVoltageReady:
				LOGIC_Wrapper_IsLeakageOutputReady(DSS_Leakage_StopOutVoltage, DSS_Leakage_StopOutVoltage,
						Timeout, &Problem);
				break;

			case DSS_Leakage_StopOutVoltage:
				LOGIC_Wrapper_StopLeakage(DSS_Leakage_WaitStopOutVoltage);
				break;

			case DSS_Leakage_WaitStopOutVoltage:
				LOGIC_Wrapper_IsLeakageNodeReady(DSS_Leakage_StopControl);
				break;

			case DSS_Leakage_StopControl:
				LOGIC_Wrapper_StopControl(DSS_Leakage_WaitStopControl);
				break;

			case DSS_Leakage_WaitStopControl:
				LOGIC_Wrapper_IsControlNodeReady(DSS_Leakage_StopPowerSupply);
				break;

			case DSS_Leakage_StopPowerSupply:
				LOGIC_Wrapper_StopPowerSupply(DSS_Leakage_WaitStopPowerSupply);
				break;

			case DSS_Leakage_WaitStopPowerSupply:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_UnCommutate);
				break;

			case DSS_Leakage_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Leakage_WaitUnCommutate);
				break;

			case DSS_Leakage_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Leakage_ReadResultPS1);
				break;

			case DSS_Leakage_ReadResultPS1:
				LOGIC_Wrapper_PowerSupply1ReadResult(DSS_Leakage_ReadResultPS2, &PS1, &Problem);
				break;

			case DSS_Leakage_ReadResultPS2:
				LOGIC_Wrapper_PowerSupply2ReadResult(DSS_Leakage_ReadResultControl, &PS2, &Problem);
				break;

			case DSS_Leakage_ReadResultControl:
				LOGIC_Wrapper_ControlReadResult(DSS_Leakage_ReadResultLeakage, &Control, &Problem);
				break;

			case DSS_Leakage_ReadResultLeakage:
				LOGIC_Wrapper_LeakageReadResult(DSS_Leakage_SaveResult, &Leakage, &Problem);
				break;

			case DSS_Leakage_SaveResult:
				if(Problem == PROBLEM_NONE)
				{
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
					LOGIC_Wrapper_PowerSupply1SaveResult(PS1);
					LOGIC_Wrapper_PowerSupply2SaveResult(PS2);
					LOGIC_Wrapper_ControlSaveResult(Control);
					LOGIC_Wrapper_LeakageSaveResult(Leakage);
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

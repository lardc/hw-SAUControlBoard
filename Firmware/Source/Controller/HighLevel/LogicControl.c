// Header
#include "LogicControl.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"

// Variables
static const VIPair ZeroVI = {0, 0};
static VIPair Control, PS1, PS2;

// Functions
void CTRL_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();
		LOGIC_Wrapper_SafetyMonitor();

		switch(CONTROL_SubState)
		{
			case DSS_Control_StartTest:
				Control = PS1 = PS2 = ZeroVI;
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Control_Commutate);
				break;

			case DSS_Control_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Control_WaitCommutation, DSS_Control_SaveResult, &Problem);
				break;

			case DSS_Control_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_StartPowerSupply);
				break;

			case DSS_Control_StartPowerSupply:
				LOGIC_Wrapper_StartPowerSupply(DSS_Control_WaitStartPowerSupply, DSS_Control_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Control_WaitStartPowerSupply:
				LOGIC_Wrapper_IsPowerSupplyOutputReady(DSS_Control_StartControl, DSS_Control_StopPowerSupply,
						Timeout, &Problem);
				break;

			case DSS_Control_StartControl:
				LOGIC_Wrapper_StartControl(DSS_Control_WaitControlReady, DSS_Control_StopPowerSupply,
						&Timeout, &Problem);
				break;

			case DSS_Control_WaitControlReady:
				LOGIC_Wrapper_IsControlOutputReady(DSS_Control_SetControlDelay, DSS_Control_StopControl,
						Timeout, &Problem, true);
				break;

			case DSS_Control_SetControlDelay:
				LOGIC_Wrapper_ControlSetDelay(DSS_Control_WaitControlDelay, DSS_Control_StopControl, &Timeout);
				break;

			case DSS_Control_WaitControlDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_Control_CheckReadyAfterDelay, Timeout);
				break;

			case DSS_Control_CheckReadyAfterDelay:
				LOGIC_Wrapper_IsControlOutputReady(DSS_Control_StopControl, DSS_Control_StopControl,
						0, &Problem, true);
				break;

			case DSS_Control_StopControl:
				LOGIC_Wrapper_StopControl(DSS_Control_WaitStopControl);
				break;

			case DSS_Control_WaitStopControl:
				LOGIC_Wrapper_IsControlNodeReady(DSS_Control_StopPowerSupply);
				break;

			case DSS_Control_StopPowerSupply:
				LOGIC_Wrapper_StopPowerSupply(DSS_Control_WaitStopPowerSupply);
				break;

			case DSS_Control_WaitStopPowerSupply:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_UnCommutate);
				break;

			case DSS_Control_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Control_WaitUnCommutate);
				break;

			case DSS_Control_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Control_ReadResultPS1);
				break;

			case DSS_Control_ReadResultPS1:
				LOGIC_Wrapper_PowerSupply1ReadResult(DSS_Control_ReadResultPS2, &PS1, &Problem);
				break;

			case DSS_Control_ReadResultPS2:
				LOGIC_Wrapper_PowerSupply2ReadResult(DSS_Control_ReadResultControl, &PS2, &Problem);
				break;

			case DSS_Control_ReadResultControl:
				LOGIC_Wrapper_ControlReadResult(DSS_Control_SaveResult, &Control, &Problem);
				break;

			case DSS_Control_SaveResult:
				if(Problem == PROBLEM_NONE)
				{
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
					LOGIC_Wrapper_ControlSaveResult(Control);
					LOGIC_Wrapper_PowerSupply1SaveResult(PS1);
					LOGIC_Wrapper_PowerSupply2SaveResult(PS2);
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

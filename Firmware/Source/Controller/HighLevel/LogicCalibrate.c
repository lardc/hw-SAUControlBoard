// Header
#include "LogicCalibrate.h"

// Includes
#include "Logic.h"
#include "Controller.h"
#include "Common.h"
#include "CommonDictionary.h"
#include "DataTable.h"
#include "DeviceObjectDictionary.h"
#include "Global.h"

// Variables
static const VIPair ZeroVI = {0, 0};
static VIPair Calibration;

// Functions
void CALIBRATE_HandleMeasurement()
{
	static Int64U Timeout;
	static uint16_t Problem = PROBLEM_NONE;

	if(CONTROL_State == DS_InProcess)
	{
		LOGIC_Wrapper_FaultControl();
		LOGIC_Wrapper_SafetyMonitor();

		switch(CONTROL_SubState)
		{
			case DSS_Calibrate_Start:
				Calibration = ZeroVI;
				Problem = PROBLEM_NONE;
				LOGIC_Wrapper_Start(DSS_Calibrate_Commutate);
				break;

			case DSS_Calibrate_Commutate:
				LOGIC_Wrapper_Commutate(DSS_Calibrate_WaitCommutation, DSS_Calibrate_SaveResult, &Problem);
				break;

			case DSS_Calibrate_WaitCommutation:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Calibrate_StartOutput);
				break;

			case DSS_Calibrate_StartOutput:
				LOGIC_Wrapper_StartCalibration(DSS_Calibrate_WaitOutputReady, DSS_Calibrate_UnCommutate,
						&Timeout, &Problem);
				break;

			case DSS_Calibrate_WaitOutputReady:
				LOGIC_Wrapper_IsCalibrationOutputReady(DSS_Calibrate_SetOutputDelay, DSS_Calibrate_StopOutput,
						Timeout, &Problem);
				break;

			case DSS_Calibrate_SetOutputDelay:
				LOGIC_Wrapper_CalibrationSetDelay(DSS_Calibrate_WaitOutputDelay, DSS_Calibrate_StopOutput, &Timeout);
				break;

			case DSS_Calibrate_WaitOutputDelay:
				LOGIC_Wrapper_SetStateAfterDelay(DSS_Calibrate_StopOutput, Timeout);
				break;

			case DSS_Calibrate_StopOutput:
				LOGIC_Wrapper_StopCalibration(DSS_Calibrate_WaitStopOutput);
				break;

			case DSS_Calibrate_WaitStopOutput:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Calibrate_UnCommutate);
				break;

			case DSS_Calibrate_UnCommutate:
				LOGIC_Wrapper_UnCommutate(DSS_Calibrate_WaitUnCommutate);
				break;

			case DSS_Calibrate_WaitUnCommutate:
				LOGIC_Wrapper_WaitAllNodesReady(DSS_Calibrate_ReadResult);
				break;

			case DSS_Calibrate_ReadResult:
				LOGIC_Wrapper_CalibrationReadResult(DSS_Calibrate_SaveResult, &Calibration, &Problem);
				break;

			case DSS_Calibrate_SaveResult:
				if(Problem == PROBLEM_NONE)
				{
					DataTable[REG_OP_RESULT] = OPRESULT_OK;
					LOGIC_Wrapper_CalibrationSaveResult(Calibration);
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

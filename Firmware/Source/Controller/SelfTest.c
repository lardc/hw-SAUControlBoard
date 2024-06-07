// Header
//
#include "SelfTest.h"

// Include
//
#include "Controller.h"
#include "LowLevel.h"
#include "DataTable.h"
#include "Delay.h"
#include "SysConfig.h"

// Definition
//
#define TIME_STAGE_DELAY			500		// мс
#define TIME_STAGE_CHECK_DELAY		100		// мс

// Variables
//
volatile DeviceSelfTestStage SELFTEST_Stage = STS_None;
volatile Int64U DelayCounter = 0;
volatile DeviceSelfTestStage CurrentStage = STS_None;
volatile DeviceSelfTestStage NextStage = STS_None;

// Functions prototypes
//
void SELFTEST_StageProcess(DeviceSelfTestStage Stage);
bool SELFTEST_IsOuputsCorrect(DeviceSelfTestStage Stage);

// Functions
//
void SELFTEST_Process()
{
	if(CONTROL_State == DS_InSelfTest)
	{
		LL_StatusLamp(Red);

		switch(SELFTEST_Stage)
		{
			case STS_None:
				LL_SwitchInputRelays(false);
				DELAY_MS(TIME_STAGE_DELAY);

				SELFTTEST_SetStage((DataTable[REG_USE_OPTICAL_BARRIER]) ? STS_OptBarier : STS_Door);
				break;

			case STS_OptBarier:
				NextStage = STS_Door;
				SELFTEST_StageProcess(STS_OptBarier);
				break;

			case STS_Door:
				NextStage = STS_Input1;
				SELFTEST_StageProcess(STS_Door);
				break;

			case STS_Input1:
				NextStage = STS_Input2;
				SELFTEST_StageProcess(STS_Input1);
				break;

			case STS_Input2:
				NextStage = STS_STOP;
				SELFTEST_StageProcess(STS_Input2);
				break;

			case STS_STOP:
				NextStage = STS_Finish;
				SELFTEST_StageProcess(STS_STOP);
				break;

			case STS_Delay:
				if(CONTROL_TimeCounter >= DelayCounter)
					SELFTTEST_SetStage(NextStage);
				break;

			case STS_CheckOutputs:
				if(SELFTEST_IsOuputsCorrect(CurrentStage))
				{
					LL_SelfTestNum(CurrentStage, false);
					DelayCounter = CONTROL_TimeCounter + TIME_STAGE_DELAY;
					SELFTTEST_SetStage(STS_Delay);
				}
				else
				{
					CONTROL_SwitchToFault(DF_SELF_TEST);
					SELFTTEST_SetStage(STS_None);
				}
				break;

			case STS_Finish:
				DataTable[REG_SELF_TEST_OP_RESULT] = OPRESULT_OK;
				SELFTTEST_SetStage(STS_None);
				LL_StatusLamp(Green);
				CONTROL_SetDeviceState(DS_None);
				break;
		}


	}
}
//------------------------------------------

bool SELFTEST_IsOuputsCorrect(DeviceSelfTestStage Stage)
{
	bool Result = false;

	switch(Stage)
	{
		case STS_STOP:
		case STS_Input2:
		case STS_Door:
			Result = !LL_ReadSafetyLine(LID_Out1) && !LL_ReadSafetyLine(LID_Out2);
			break;

		case STS_OptBarier:
		case STS_Input1:
			Result = !LL_ReadSafetyLine(LID_Out1) && LL_ReadSafetyLine(LID_Out2);
			break;

		default:
			break;
	}

	return Result;
}
//------------------------------------------

void SELFTEST_StageProcess(DeviceSelfTestStage Stage)
{
	if(LL_ReadSafetyLine(LID_Out1) && LL_ReadSafetyLine(LID_Out2))
	{
		if(LL_MEASURE_OutputVoltage(ADC1_OUTPUT1) >= OUTPUT_THRESHOLD_VOLTAGE)
			CONTROL_SwitchToFault(DF_SHORT_OUTPUT1);
		else
		{
			if(LL_MEASURE_OutputVoltage(ADC1_OUTPUT2) >= OUTPUT_THRESHOLD_VOLTAGE)
				CONTROL_SwitchToFault(DF_SHORT_OUTPUT2);
			else
			{
				DelayCounter = CONTROL_TimeCounter + TIME_STAGE_DELAY;
				LL_SelfTestNum(Stage, true);
				CurrentStage = Stage;
				DataTable[REG_SELF_TEST_STAGE] = Stage;

				DELAY_MS(TIME_STAGE_CHECK_DELAY);
				SELFTTEST_SetStage(STS_CheckOutputs);
			}
		}
	}
	else
	{
		CONTROL_SwitchToFault(DF_SELF_TEST);
		SELFTTEST_SetStage(STS_None);
	}
}
//------------------------------------------

void SELFTTEST_SetStage(DeviceSelfTestStage NewStage)
{
	SELFTEST_Stage = NewStage;
}
//------------------------------------------

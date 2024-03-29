// Header
#include "LowLevel.h"

// Include
#include "Board.h"
#include "Delay.h"
#include "SysConfig.h"
#include "Global.h"

// Functions
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_INT_LED);
}
//-----------------------------

void LL_SelfTestNum(DeviceSelfTestStage Stage, bool State)
{
	GPIO_SetState(GPIO_TEST1, false);
	GPIO_SetState(GPIO_TEST2, false);
	GPIO_SetState(GPIO_TEST3, false);
	GPIO_SetState(GPIO_TEST4, false);
	GPIO_SetState(GPIO_TEST5, false);

	switch(Stage)
	{
		case STS_OptBarier:
			GPIO_SetState(GPIO_TEST1, State);
			break;

		case STS_Door:
			GPIO_SetState(GPIO_TEST2, State);
			break;

		case STS_Input1:
			GPIO_SetState(GPIO_TEST3, State);
			break;

		case STS_STOP:
			GPIO_SetState(GPIO_TEST4, State);
			break;

		case STS_Input2:
			GPIO_SetState(GPIO_TEST5, State);
			break;

		default:
			break;
	}
}
//-----------------------------

void LL_StatusLamp(ColorLamp Color)
{
	if(Color == Red)
	{
		GPIO_SetState(GPIO_ST_LAMP_GREEN, false);
		GPIO_SetState(GPIO_ST_LAMP_RED, true);
	}

	if(Color == Green)
	{
		GPIO_SetState(GPIO_ST_LAMP_GREEN, true);
		GPIO_SetState(GPIO_ST_LAMP_RED, false);
	}

	if(Color == SwitchedOff)
	{
		GPIO_SetState(GPIO_ST_LAMP_GREEN, false);
		GPIO_SetState(GPIO_ST_LAMP_RED, false);
	}
}
//-----------------------------

void LL_ToggleSwitchLamp(LineID Lamp, bool State)
{
	switch(Lamp)
	{
		case LID_OptBarier:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP1) : GPIO_SetState(GPIO_SW_LAMP1, false);
			break;

		case LID_Door:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP2) : GPIO_SetState(GPIO_SW_LAMP2, false);
			break;

		case LID_Input:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP3) : GPIO_SetState(GPIO_SW_LAMP3, false);
			break;

		default:
			break;
	}
}
//-----------------------------

bool LL_ReadSafetyLine(LineID Line)
{
	switch(Line)
	{
		case LID_OptBarier:
			return GPIO_GetState(GPIO_S1);
			break;

		case LID_Door:
			return GPIO_GetState(GPIO_S2);
			break;

		case LID_Input:
			return GPIO_GetState(GPIO_S3);
			break;

		case LID_Out1:
			return GPIO_GetState(GPIO_S4);
			break;

		case LID_Out2:
			return GPIO_GetState(GPIO_S5);
			break;

		default:
			break;
	}

	return 0;
}
//-----------------------------

bool LL_ReadTemperatureFlag()
{
	return GPIO_GetState(GPIO_TEMPERATURE);
}
//-----------------------------

float LL_MEASURE_OutputVoltage(Int16U ADC1Channel)
{
	return (float)ADC_Measure(ADC1, ADC1Channel) * ADC_REF_VOLTAGE / ADC_RESOLUTION;
}
//------------------------------------------------------------------------------

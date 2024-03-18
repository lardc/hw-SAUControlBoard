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

void LL_SelfTestNum(SafetyLine Line, bool State)
{
	GPIO_SetState(GPIO_TEST1, false);
	GPIO_SetState(GPIO_TEST2, false);
	GPIO_SetState(GPIO_TEST3, false);
	GPIO_SetState(GPIO_TEST4, false);
	GPIO_SetState(GPIO_TEST5, false);

	switch(Line)
	{
		case SF_OptBarierSwitch:
			GPIO_SetState(GPIO_TEST1, State);
			break;

		case SF_DoorSwitch:
			GPIO_SetState(GPIO_TEST2, State);
			break;

		case SF_InputSwitch:
			GPIO_SetState(GPIO_TEST3, State);
			break;

		case SF_Out1:
			GPIO_SetState(GPIO_TEST4, State);
			break;

		case SF_Out2:
			GPIO_SetState(GPIO_TEST5, State);
			break;

		default:
			break;
	}
}
//-----------------------------

void LL_LampGreen(bool State)
{
	GPIO_SetState(GPIO_ST_LAMP_GREEN, State);
}
//-----------------------------

void LL_LampRed(bool State)
{
	GPIO_SetState(GPIO_ST_LAMP_RED, State);
}
//-----------------------------

void LL_ToggleSwitchLamp(SwitchLamp Lamp, bool State)
{
	switch(Lamp)
	{
		case Lamp_OptBarierSwitch:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP1) : GPIO_SetState(GPIO_SW_LAMP1, false);
			break;

		case Lamp_DoorSwitch:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP2) : GPIO_SetState(GPIO_SW_LAMP2, false);
			break;

		case Lamp_InputSwitch:
			(State) ? GPIO_Toggle(GPIO_SW_LAMP3) : GPIO_SetState(GPIO_SW_LAMP3, false);
			break;

		default:
			break;
	}
}
//-----------------------------

bool LL_ReadSafetyLine(SafetyLine Line)
{
	switch(Line)
	{
		case SF_OptBarierSwitch:
			return GPIO_GetState(GPIO_S1);
			break;

		case SF_DoorSwitch:
			return GPIO_GetState(GPIO_S2);
			break;

		case SF_InputSwitch:
			return GPIO_GetState(GPIO_S3);
			break;

		case SF_Out1:
			return GPIO_GetState(GPIO_S4);
			break;

		case SF_Out2:
			return GPIO_GetState(GPIO_S5);
			break;

		default:
			break;
	}

	return 0;
}
//-----------------------------

float LL_MEASURE_VoltageX(Int16U ADC1Channel)
{
	return (float)ADC_Measure(ADC1, ADC1Channel) * ADC_REF_VOLTAGE / ADC_RESOLUTION;
}
//------------------------------------------------------------------------------

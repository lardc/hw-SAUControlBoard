// Header
#include "LowLevel.h"

// Include
#include "Board.h"
#include "Delay.h"

// Functions
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_INT_LED);
}
//-----------------------------

void LL_Fan(bool State)
{
	GPIO_SetState(GPIO_INT_FAN, State);
}
//-----------------------------

void LL_LampGreen(bool State)
{
	GPIO_SetState(GPIO_LAMP_GREEN, State);
}
//-----------------------------

void LL_LampRed(bool State)
{
	GPIO_SetState(GPIO_LAMP_RED, State);
}
//-----------------------------

bool LL_ExternalButton()
{
	return GPIO_GetState(GPIO_EXT_BUTTON);
}
//-----------------------------

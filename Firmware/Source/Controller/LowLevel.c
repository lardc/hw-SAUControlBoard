// Header
#include "LowLevel.h"

// Include
#include "Board.h"
#include "Delay.h"

// Functions
//
// LED on board
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_INT_LED);
}
//------------------------------------------------------------------------------
//
// FAN
void LL_INT_FAN(bool State)
{
	GPIO_SetState(GPIO_INT_FAN, State);
}
//------------------------------------------------------------------------------
//
// External lamp GREEN
void LL_ExternalLampGREEN(bool State)
{
	GPIO_SetState(GPIO_LAMP_GREEN, State);
}
//------------------------------------------------------------------------------
//
// External lamp RED
void LL_ExternalLampRED(bool State)
{
	GPIO_SetState(GPIO_LAMP_RED, State);
}
//------------------------------------------------------------------------------
// Button PC
void LL_PC_SWITCH()
{
	GPIO_SetState(GPIO_PC_SWITCH, TRUE);
		DELAY_US(500000);
	GPIO_SetState(GPIO_PC_SWITCH, FALSE);
}


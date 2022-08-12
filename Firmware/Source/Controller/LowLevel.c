// Header
#include "LowLevel.h"
// Include
#include "Board.h"
#include "Delay.h"

// Functions
//
void LL_ToggleBoardLED()
{
	GPIO_Toggle(GPIO_INT_LED);
}
//-----------------------------

void LL_SetStateExtLed(bool State)
{
	GPIO_SetState(GPIO_EXT_LED, State);
}
//-----------------------------

void LL_SetStateFan(bool State)
{
	GPIO_SetState(GPIO_FAN, State);
}
//-----------------------------

void LL_SetStateExtLineSync1(bool State)
{
	GPIO_SetState(GPIO_EXT_SYNC1_OUT, State);
}
//-----------------------------

void LL_SetStateExtLineSync2(bool State)
{
	GPIO_SetState(GPIO_EXT_SYNC2_OUT, State);
}
//-----------------------------

void LL_SetStateIntLineSync1(bool State)
{
	GPIO_SetState(GPIO_INT_SYNC1_OUT, State);
}
//-----------------------------

void LL_SetStateIntLineSync2(bool State)
{
	GPIO_SetState(GPIO_INT_SYNC2_OUT, State);
}
//-----------------------------

bool LL_GetStateIntLineSync1()
{
	return GPIO_GetState(GPIO_INT_SYNC1_IN);
}
//-----------------------------

bool LL_GetStateIntLineSync2()
{
	return GPIO_GetState(GPIO_INT_SYNC2_IN);
}
//-----------------------------

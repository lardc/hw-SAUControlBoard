#include "InitConfig.h"
#include "Board.h"
#include "SysConfig.h"
#include "BCCIxParams.h"

// Functions
//
Boolean INITCFG_ConfigSystemClock()
{
	return RCC_PLL_HSE_Config(QUARTZ_FREQUENCY, PREDIV_4, PLL_14);
}
//------------------------------------------------

void INITCFG_ConfigIO()
{
	// Включение тактирования портов
	RCC_GPIO_Clk_EN(PORTA);
	RCC_GPIO_Clk_EN(PORTB);
	
	// Выходы
	GPIO_InitPushPullOutput(GPIO_INT_LED);
	GPIO_InitPushPullOutput(GPIO_EXT_LED);
	GPIO_InitPushPullOutput(GPIO_INT_SYNC1_OUT);
	GPIO_InitPushPullOutput(GPIO_INT_SYNC2_OUT);
	GPIO_InitPushPullOutput(GPIO_EXT_SYNC1_OUT);
	GPIO_InitPushPullOutput(GPIO_EXT_SYNC2_OUT);
	GPIO_InitPushPullOutput(GPIO_FAN);

	// Входы
	GPIO_InitInput(GPIO_INT_SYNC1_IN, NoPull);
	GPIO_InitInput(GPIO_INT_SYNC2_IN, NoPull);

	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART1_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART1_TX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART2_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART2_TX, AltFn_7);
}
//------------------------------------------------

void INITCFG_ConfigCAN()
{
	RCC_CAN_Clk_EN(CAN_1_ClkEN);
	NCAN_Init(SYSCLK, CAN_BAUDRATE, false);
	NCAN_FIFOInterrupt(true);
	NCAN_FilterInit(0, CAN_SLAVE_FILTER_ID, CAN_SLAVE_NID_MASK);
	NCAN_FilterInit(1, CAN_MASTER_FILTER_ID, CAN_MASTER_NID_MASK);
}
//------------------------------------------------

void INITCFG_ConfigUART()
{
	USARTx_Init(USART1, SYSCLK, USART_BAUDRATE);
	USARTx_RecieveInterrupt(USART1, true);

	USARTx_Init(USART2, SYSCLK, USART_BAUDRATE);
	USARTx_RecieveInterrupt(USART2, true);
}
//------------------------------------------------

void INITCFG_ConfigTimer7()
{
	TIM_Clock_En(TIM_7);
	TIM_Config(TIM7, SYSCLK, TIMER7_uS);
	TIM_Interupt(TIM7, 0, true);
	TIM_Start(TIM7);
}
//------------------------------------------------

void INITCFG_ConfigWatchDog()
{
	IWDG_Config();
	IWDG_ConfigureSlowUpdate();
}
//------------------------------------------------

void INITCFG_Sync2Proxy()
{
	EXTI_Config(EXTI_PB, EXTI_10, BOTH_TRIG, 0);
	EXTI_EnableInterrupt(EXTI15_10_IRQn, 0, true);
}
//------------------------------------------------

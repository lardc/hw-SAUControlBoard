#include "InitConfig.h"
#include "Board.h"
#include "SysConfig.h"
#include "BCCIxParams.h"

// Functions
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
	GPIO_InitPushPullOutput(GPIO_TEST1);
	GPIO_InitPushPullOutput(GPIO_TEST2);
	GPIO_InitPushPullOutput(GPIO_TEST3);
	GPIO_InitPushPullOutput(GPIO_TEST4);
	GPIO_InitPushPullOutput(GPIO_TEST5);
	GPIO_InitPushPullOutput(GPIO_ST_LAMP_RED);
	GPIO_InitPushPullOutput(GPIO_ST_LAMP_GREEN);
	GPIO_InitPushPullOutput(GPIO_SW_LAMP1);
	GPIO_InitPushPullOutput(GPIO_SW_LAMP2);
	GPIO_InitPushPullOutput(GPIO_SW_LAMP3);

	// Входы
	GPIO_InitInput(GPIO_S1, NoPull);
	GPIO_InitInput(GPIO_S2, NoPull);
	GPIO_InitInput(GPIO_S3, NoPull);
	GPIO_InitInput(GPIO_S4, NoPull);
	GPIO_InitInput(GPIO_S5, NoPull);
	GPIO_InitInput(GPIO_TEMPERATURE, NoPull);

	// Начальная установка портов
	GPIO_SetState(GPIO_INT_LED, false);
	GPIO_SetState(GPIO_TEST1, false);
	GPIO_SetState(GPIO_TEST2, false);
	GPIO_SetState(GPIO_TEST3, false);
	GPIO_SetState(GPIO_TEST4, false);
	GPIO_SetState(GPIO_TEST5, false);
	GPIO_SetState(GPIO_ST_LAMP_RED, false);
	GPIO_SetState(GPIO_ST_LAMP_GREEN, false);
	GPIO_SetState(GPIO_SW_LAMP1, false);
	GPIO_SetState(GPIO_SW_LAMP2, false);
	GPIO_SetState(GPIO_SW_LAMP3, false);

	// Альтернативные функции
	GPIO_InitAltFunction(GPIO_ALT_CAN_RX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_CAN_TX, AltFn_9);
	GPIO_InitAltFunction(GPIO_ALT_UART1_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART1_TX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART3_RX, AltFn_7);
	GPIO_InitAltFunction(GPIO_ALT_UART3_TX, AltFn_7);
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

	USARTx_Init(USART3, SYSCLK, USART_BAUDRATE);
	USARTx_RecieveInterrupt(USART3, true);
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

#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwUSART.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"
#include "ZwSPI.h"
#include "Delay.h"

// Определения аналоговых портов
GPIO_PortPinSettingMacro GPIO_MEASURE_CH1		= {GPIOA, Pin_0};
GPIO_PortPinSettingMacro GPIO_MEASURE_CH2		= {GPIOA, Pin_1};

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_INT_LED			= {GPIOB, Pin_1};
GPIO_PortPinSettingMacro GPIO_TEST1				= {GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_TEST2				= {GPIOA, Pin_4};
GPIO_PortPinSettingMacro GPIO_TEST3				= {GPIOA, Pin_3};
GPIO_PortPinSettingMacro GPIO_TEST4				= {GPIOA, Pin_5};
GPIO_PortPinSettingMacro GPIO_TEST5				= {GPIOB, Pin_2};
GPIO_PortPinSettingMacro GPIO_ST_LAMP_RED		= {GPIOA, Pin_15};
GPIO_PortPinSettingMacro GPIO_ST_LAMP_GREEN		= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_SW_LAMP1			= {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_SW_LAMP2			= {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_SW_LAMP3			= {GPIOB, Pin_4};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_S1				= {GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_S2				= {GPIOA, Pin_7};
GPIO_PortPinSettingMacro GPIO_S3				= {GPIOA, Pin_6};
GPIO_PortPinSettingMacro GPIO_S4				= {GPIOB, Pin_9};
GPIO_PortPinSettingMacro GPIO_S5				= {GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_TEMPERATURE		= {GPIOA, Pin_8};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX		= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX		= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_TX		= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_RX		= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART3_TX		= {GPIOB, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART3_RX		= {GPIOB, Pin_11};

#endif // __BOARD_H

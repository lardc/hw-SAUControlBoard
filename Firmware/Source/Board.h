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

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_INT_LED			= {GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_INT_FAN			= {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_LAMP_GREEN		= {GPIOB, Pin_7};
GPIO_PortPinSettingMacro GPIO_LAMP_RED			= {GPIOB, Pin_8};
GPIO_PortPinSettingMacro GPIO_PC_SWITCH			= {GPIOB, Pin_5};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_EXT_BUTTON		= {GPIOA, Pin_3};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX		= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX		= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_TX		= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_RX		= {GPIOA, Pin_10};
GPIO_PortPinSettingMacro GPIO_ALT_UART2_TX		= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_ALT_UART2_RX		= {GPIOB, Pin_4};

#endif // __BOARD_H

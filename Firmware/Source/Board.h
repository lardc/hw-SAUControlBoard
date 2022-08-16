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

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_EXT_SYNC1_OUT		= {GPIOA, Pin_3};
GPIO_PortPinSettingMacro GPIO_EXT_SYNC2_OUT		= {GPIOA, Pin_4};
GPIO_PortPinSettingMacro GPIO_EXT_LED			= {GPIOA, Pin_5};
GPIO_PortPinSettingMacro GPIO_FAN				= {GPIOA, Pin_7};
GPIO_PortPinSettingMacro GPIO_INT_SYNC2_OUT		= {GPIOB, Pin_0};
GPIO_PortPinSettingMacro GPIO_INT_SYNC1_OUT		= {GPIOB, Pin_1};
GPIO_PortPinSettingMacro GPIO_INT_LED			= {GPIOB, Pin_7};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_INT_SYNC1_IN		= {GPIOB, Pin_2};
GPIO_PortPinSettingMacro GPIO_INT_SYNC2_IN		= {GPIOB, Pin_10};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX		= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX		= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_TX		= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_RX		= {GPIOA, Pin_10};

GPIO_PortPinSettingMacro GPIO_ALT_UART2_TX		= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_ALT_UART2_RX		= {GPIOB, Pin_4};

#endif // __BOARD_H

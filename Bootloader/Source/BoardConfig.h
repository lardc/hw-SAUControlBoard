﻿// -----------------------------------------
// Board parameters
// ----------------------------------------

#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

// Board includes
//
#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwUSART.h"
#include "ZwTIM.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"

// Definitions
//
// Blinking LED settings
#define LED_BLINK_PORT		GPIOB
#define LED_BLINK_PIN		Pin_1

#endif // __BOARD_CONFIG_H

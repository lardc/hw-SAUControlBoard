﻿#ifndef __GLOBAL_H
#define __GLOBAL_H

// Include
#include "stdinc.h"

// Definitions
#define MME_CODE						310

#define	xCCI_TIMEOUT_TICKS				2000
#define EP_COUNT						0
#define EP_WRITE_COUNT					0
#define FEP_COUNT						0
#define ENABLE_LOCKING					FALSE
//
#define TIME_LED_BLINK					500		// Мигание светодиодом (в мс)
//
#define ADC_REF_VOLTAGE					3300	// мВ
#define ADC_RESOLUTION					4095
//
#define SAFETY_SWITCH_NUM				3		// Количество выключателей безопасности
#define TIME_SAFETY_SWITCH_BLINK		250		// Период моргания индикатора выключателя безопасности, мс
#define OUTPUT_THRESHOLD_VOLTAGE		2000	// Уровень порогового напряжения на выходах безопасности
#define TIME_FAULT_LED_BLINK			250		// Период моргания красного индикатора в состоянии Fault


#endif // __GLOBAL_H

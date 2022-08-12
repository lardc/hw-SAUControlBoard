#ifndef __MULTIPLEXER_DICTIONARY_H
#define __MULTIPLEXER_DICTIONARY_H

// Команды
#define MUX_ACT_SET_RELAY_GROUP				100	// Подключить группу реле (на основании корпуса и т.д.)
#define MUX_ACT_SET_RELAY_NONE				101	// Отключить все релле
#define MUX_ACT_SET_RELAY_GROUP_FAST_HV		102	// Быстрое подключение высоковольтных реле

// Регистры
#define MUX_REG_TYPE_MEASURE				130	// Тип измерения
#define MUX_REG_TYPE_CASE					131	// Тип корпуса
#define MUX_REG_POSITION_OF_CASE			132	// Позиция
#define MUX_REG_TYPE_SIGNAL_CTRL			133	// Тип управления
#define MUX_REG_TYPE_LEAKAGE				134	// Тип сигнала при утечке
#define MUX_REG_TYPE_POLARITY				135	// Полярность подключения
#define MUX_REG_ENABLE_SAFETY				136	// Разрешение контура безопасности при включённой коммутации

#define MUX_REG_BUTTON_START				210	// Регистр состояния кнопки старт

// Дополнительные состояния
#define MUX_STATE_SAFETY_TRIG				5

typedef enum __MUX_Position
{
	MXP_Position1 = 1,
	MXP_Position2 = 2,
	MXP_Position3 = 3
} MUX_Position;

typedef enum __MUX_OutputPolarity
{
	MXOP_Direct = 1,
	MXOP_Reverse = 2
} MUX_Polarity;

typedef enum __MUX_Control
{
	MXC_ControlDC = 1,
	MXC_ControlAC = 2,
	MXC_ControlDCReverse = 3
} MUX_Control;

#endif // __MULTIPLEXER_DICTIONARY_H

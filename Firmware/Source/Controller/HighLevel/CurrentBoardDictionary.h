#ifndef __CURRENT_BOARD_DICTIONARY_H
#define __CURRENT_BOARD_DICTIONARY_H

// Команды
#define CURR_ACT_START_PROCESS				100	// Страрт процесса формирования импульса
#define CURR_ACT_STOP_PROCESS				101	// Остановка процесса формирования импульса

// Регистры
#define CURR_REG_CURRENT_SETPOINT			128	// Значение задания тока (в мкА)
#define CURR_REG_CURRENT_SETPOINT_32		129
#define CURR_REG_VOLTAGE_DUT_LIM			130	// Предельное значение падения напряжения (в мкВ)
#define CURR_REG_VOLTAGE_DUT_LIM_32			131

#define CURR_REG_AFTER_PULSE_DELAY			200	// Индикация выдерживания паузы между импульсами (1 - пауза активна)

#define CURR_REG_RESULT_CURRENT				250	// Полученное значение тока (в мкА)
#define CURR_REG_RESULT_CURRENT_32			251
#define CURR_REG_RESULT_VOLTAGE				252	// Полученное значение напряжения (в мкВ)
#define CURR_REG_RESULT_VOLTAGE_32			253

typedef enum __CurrentShape
{
	CB_CurrentTrapeze = 1,
	CB_CurrentSin = 2
} CurrentShape;

#endif // __CURRENT_BOARD_DICTIONARY_H

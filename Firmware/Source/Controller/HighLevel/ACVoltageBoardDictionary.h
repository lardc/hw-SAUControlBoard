#ifndef __AC_VOLTAGE_BOARD_DICT_H
#define __AC_VOLTAGE_BOARD_DICT_H

// Команды
#define ACV_ACT_START_PROCESS				100	// Страрт процесса формирования напряяжения
#define ACV_ACT_STOP_PROCESS				101	// Остановка процесса формирования напряяжения

// Регистры
#define ACV_REG_VOLTAGE_SETPOINT			128	// Значение задания напряжения (в мВ)
#define ACV_REG_VOLTAGE_SETPOINT_32			129
#define ACV_REG_CURRENT_SETPOINT			130	// Значение задания тока (в мкА)
#define ACV_REG_CURRENT_SETPOINT_32			131
#define ACV_REG_OUTPUT_LINE					132	// Линия подачи выходного напряжения

#define ACV_REG_VOLTAGE_RESULT				200	// Значение измерения напряжения (в мВ)
#define ACV_REG_VOLTAGE_RESULT_32			201
#define ACV_REG_CURRENT_RESULT				202	// Значение измерения тока (в мкА)
#define ACV_REG_CURRENT_RESULT_32			203
#define ACV_REG_VOLTAGE_READY				204	// Флаг выхода на уставку напряжения

typedef enum __ACV_OutputLine
{
	ACV_BUS_LV = 1,
	ACV_CTRL = 2
} ACV_OutputLine;

#endif // __AC_VOLTAGE_BOARD_DICT_H

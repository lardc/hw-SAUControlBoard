#ifndef __DC_VOLTAGE_BOARD_DICT_H
#define __DC_VOLTAGE_BOARD_DICT_H

// Команды
#define DCV_ACT_START_PROCESS				100	// Страрт процесса формирования напряяжения
#define DCV_ACT_STOP_PROCESS				101	// Остановка процесса формирования напряяжения

// Регистры
#define DCV_REG_OUTPUT_LINE					128	// Линия подачи выходного напряжения
#define DCV_REG_OUTPUT_TYPE					129	// Тип выхода
#define DCV_REG_OUTPUT_MODE					130	// Режим выхода
#define DCV_REG_PULSE_LENGTH				131	// Длительность выходного импульса

#define DCV_REG_CURRENT_SETPOINT			135	// Значение задания тока
#define DCV_REG_CURRENT_SETPOINT_32			136
#define DCV_REG_VOLTAGE_SETPOINT			137	// Значение задания напряжения
#define DCV_REG_VOLTAGE_SETPOINT_32			138

#define DCV_REG_CURRENT_RESULT				200	// Значение измерения тока
#define DCV_REG_CURRENT_RESULT_32			201
#define DCV_REG_VOLTAGE_RESULT				202	// Значение измерения напряжения
#define DCV_REG_VOLTAGE_RESULT_32			203

#define DCV_REG_VOLTAGE_READY				205	// Флаг выхода на уставку напряжения

typedef enum __DCV_OutputLine
{
	DCV_BUS_LV = 1,
	DCV_CTRL = 2,
	DCV_PS1 = 3,
	DCV_PS2 = 4
} DCV_OutputLine;

typedef enum __DCV_OutputType
{
	DCV_Voltage = 1,
	DCV_Current = 2
} DCV_OutputType;

typedef enum __DCV_OutputMode
{
	DCV_Pulse = 1,
	DCV_Continuous = 2
} DCV_OutputMode;

#endif // __DC_VOLTAGE_BOARD_DICT_H

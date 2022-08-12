#ifndef __DC_HIGH_VOLTAGE_BOARD_DICT_H
#define __DC_HIGH_VOLTAGE_BOARD_DICT_H

// Команды
#define DCHV_ACT_START_PROCESS				100	// Страрт процесса формирования напряяжения
#define DCHV_ACT_STOP_PROCESS				101	// Остановка процесса формирования напряяжения
#define DCHV_ACT_SECOND_START_PROCESS		102	// Команда запуска второго этапа формирования импульса

// Регистры
#define DCHV_REG_VOLTAGE_SETPOINT			128	// Значение задания напряжения
#define DCHV_REG_CURRENT_SETPOINT			129
#define DCHV_REG_CURRENT_SETPOINT_32		130	// Значение задания тока

#define DCHV_REG_VOLTAGE_RESULT				200	// Значение измерения напряжения
#define DCHV_REG_CURRENT_RESULT				201
#define DCHV_REG_CURRENT_RESULT_32			202	// Значение измерения тока

// Дополнительные состояния
#define DCHV_STATE_IN_PROCESS_EX			5

#endif // __DC_HIGH_VOLTAGE_BOARD_DICT_H

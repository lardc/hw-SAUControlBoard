#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Commands

#define ACT_DIAG_SELFTEST_RELAY					10	// Проверка реле самодиагностики
#define ACT_DIAG_SWITCH							11	// Проверка выключателей безопасности
#define ACT_DIAG_GREEN_LED						12	// Проверка зеленого индикатора
#define ACT_DIAG_RED_LED						13	// Проверка красного индикатора
//
#define ACT_SET_ACTIVE							100	// Команда активации контура безопасности
#define ACT_SET_INACTIVE						101	// Команда деактивации контура безопасности
//
#define ACT_START_SELF_TEST						110	// Запуск самотестирования
//
#define ACT_SAVE_TO_ROM							200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM					201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT					202	// Сброс DataTable в состояние по умолчанию
//
#define ACT_BOOT_LOADER_REQUEST					320	// Request reboot to bootloader

// Registers
//
#define REG_STATUS_INDICATION					128	// 0 - зеленый, 1 - красный
//
#define REG_DBG									150	// Отладочный регистр
//
#define REG_DEV_STATE							192	// Регистр состояния
#define REG_FAULT_REASON						193	// Регистр Fault
#define REG_DISABLE_REASON						194	// Регистр Disable
#define REG_WARNING								195	// Регистр Warning
#define REG_PROBLEM								196	// Регистр Problem
#define REG_OP_RESULT							197	// Регистр результата операции
#define REG_SELF_TEST_OP_RESULT					198	// Регистр результата самотестирования
#define REG_SUB_STATE							199	// Регистр вспомогательного состояния
//
#define REG_TEMPERATURE_FLAG					200	// 0 - безопасная температура, 1 - температура выше безопасного уровня
//
#define REG_OVERLAP_COUNT_REQ					210	// Overlapping requests via SCCI interface
#define REG_OVERLAP_COUNT_RESP					211	// Overlapping responses via SCCI interface
//
#define REG_MME_CODE							250	// MME code number
//
#define REG_FWINFO_SLAVE_NID					256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID					257	// Device CAN master node ID (if presented)
//
#define REG_FWINFO_STR_LEN						260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN					261	// Begining of the information string record

// Fault and disable codes
//
#define DF_NONE
#define DF_SHORT_OUTPUT1						1	// КЗ на выходе 1
#define DF_SHORT_OUTPUT2						2	// КЗ на выходе 2

//  User Errors
//
#define ERR_NONE								0
#define ERR_CONFIGURATION_LOCKED				1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED					2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY					3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD							4	//  Неправильный ключ

#endif // __DEV_OBJ_DIC_H

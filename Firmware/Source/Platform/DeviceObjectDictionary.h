#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Commands

#define ACT_DIAG_SELFTEST_RELAY		10	// Проверка реле самодиагностики
#define ACT_DIAG_SWITCH				11	// Проверка выключателей безопасности
#define ACT_DIAG_GREEN_LED			12	// Проверка зеленого индикатора
#define ACT_DIAG_RED_LED			13	// Проверка красного индикатора
//
#define ACT_SAVE_TO_ROM				200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM		201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT		202	// Сброс DataTable в состояние по умолчанию
//
#define ACT_BOOT_LOADER_REQUEST		320	// Request reboot to bootloader

// Registers
#define REG_DBG						150	// Отладочный регистр
//
#define REG_DEV_STATE				192	// Device state (Не используется, для совместимости)
#define REG_FAULT_REASON			193	// Fault reason in the case DeviceState -> FAULT (Не используется, для совместимости)
#define REG_DISABLE_REASON			194	// Fault reason in the case DeviceState -> DISABLED (Не используется, для совместимости)
#define REG_WARNING					195	// Warning if present (Не используется, для совместимости)
//

//
#define REG_OVERLAP_COUNT_REQ		210	// Overlapping requests via SCCI interface
#define REG_OVERLAP_COUNT_RESP		211	// Overlapping responses via SCCI interface
//
#define REG_MME_CODE				250	// MME code number

#define REG_FWINFO_SLAVE_NID		256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID		257	// Device CAN master node ID (if presented)
//
#define REG_FWINFO_STR_LEN			260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN		261	// Begining of the information string record

#endif // __DEV_OBJ_DIC_H

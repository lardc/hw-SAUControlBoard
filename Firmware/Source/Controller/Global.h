#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS			1000	// Таймаут интерфейса SCCI (в мс)
#define	BCCIM_TIMEOUT_TICKS			1000	// Таймаут протоколоа мастер BCCI (в мс)
#define EP_WRITE_COUNT				0		// Количество массивов для записи
#define EP_COUNT					1		// Количество массивов для чтения
#define ENABLE_LOCKING				FALSE	// Защита NV регистров паролем
#define XCCI_EP_SIZE				2000	// Размер массива для вычитывания EP

// Временные параметры
#define TIME_LED_BLINK				500		// Мигание светодиодом (в мс)
#define TIME_SLAVE_STATE_UPDATE		100		// Интервал опроса состояний ведомых узлов (в мс)

#define MAX_SLAVE_NODES				8		// Максимальное чилсо slave-узлов

#endif //  __GLOBAL_H

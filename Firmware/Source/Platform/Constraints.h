#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

// Definitions
#define NO		0
#define YES		1

#define NODE_ID_MIN					1
#define NODE_ID_MAX					255

// в мс
#define TIMEOUT_MIN					1000
#define TIMEOUT_MAX					60000
#define TIMEOUT_DEF					5000

// в мс
#define CTRL_PULSE_LENGH_MIN		5
#define CTRL_PULSE_LENGH_MAX		500
#define CTRL_PULSE_LENGH_DEF		10

// в мс
#define CTRL_DELAY_MIN				0
#define CTRL_DELAY_MAX				2000
#define CTRL_DELAY_DEF				300

// в мс
#define CALIBRATION_DELAY_MIN		0
#define CALIBRATION_DELAY_MAX		10000
#define CALIBRATION_DELAY_DEF		1000

// в сек.
#define FAN_TIME_MIN				10
#define FAN_TIME_MAX				600
#define FAN_TIME_DEF				30

// в мс
#define ALTER_DELAY_MIN				0
#define ALTER_DELAY_MAX				10000
#define ALTER_DELAY_DEF				0

// Types
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H

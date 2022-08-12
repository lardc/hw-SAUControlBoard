#ifndef __COMMON_H
#define __COMMON_H

// Include
#include "stdinc.h"
#include "CommonDictionary.h"
#include "DUTLibrary.h"

// Types
//
typedef enum __NodeName
{
	NAME_Multiplexer = 0,
	NAME_DCCurrent = 1,
	NAME_DCHighVoltage = 2,
	NAME_DCVoltage1 = 3,
	NAME_DCVoltage2 = 4,
	NAME_DCVoltage3 = 5,
	NAME_ACVoltage1 = 6,
	NAME_ACVoltage2 = 7
} NodeName;

typedef enum __MeasurementType
{
	MT_LeakageCurrent = 1,
	MT_OnVoltage = 2,
	MT_InputVoltageCurrent = 3,
	MT_InhibitVoltage = 4,

	MT_CalibrateDCControl = 5,
	MT_CalibratePS1 = 6,
	MT_CalibratePS2 = 7,
	MT_CalibrateDCLeakage = 8,
	MT_CalibrateACLeakage = 9,
	MT_CalibrateACControl = 10,
	MT_CalibrateCurrent = 11
} MeasurementType, *pMeasurementType;

typedef enum __ExecutionResult
{
	ER_NoError = 0,
	ER_InterfaceError = 1,
	ER_SettingsError = 2,
	ER_WrongNode = 3,
	ER_ChangeStateTimeout = 4,
	ER_WrongState = 5,
	ER_BadHighLevelConfig = 6
} ExecutionResult;

typedef enum __LeakageType
{
	LT_LeakageDC = 1,
	LT_LeakageAC = 2
} LeakageType;

typedef enum __InputType
{
	IT_ControlIDC = 1,
	IT_ControlVDC = 2,
	IT_ControlVAC = 3
} InputType;

typedef enum __GetSlaveStateResult
{
	GSSR_Emulation = 1,
	GSSR_Outdated = 2,
	GSSR_Equal = 3,
	GSSR_NotEqual = 4
} GetSlaveStateResult;

typedef struct __SlaveNode
{
	bool Emulation;
	bool StateIsUpToDate;
	uint16_t NodeID;
	uint16_t State;
	uint16_t OpResult;
	void *Settings;
} SlaveNode, *pSlaveNode;

typedef struct __VIPair
{
	uint32_t Voltage;
	uint32_t Current;
} VIPair, *pVIPair;

// Functions
//
void COMM_InitSlaveArray();
void COMM_UpdateEmulationSettings();

bool COMM_SlavesReadState();
bool COMM_AreSlavesInStateX(uint16_t State);
GetSlaveStateResult COMM_IsSlaveInStateX(NodeName Name, uint16_t State);
uint16_t COMM_GetSlaveOpResult(NodeName Name);
bool COMM_IsSlaveInFaultOrDisabled();
void COMM_ForceEmulationByNodeID(uint16_t NodeID);

bool COMM_SlavesEnablePower();
bool COMM_SlavesDisablePower();
bool COMM_SlavesClearFault();
bool COMM_SlavesClearWarning();

pSlaveNode COMM_GetSlaveDevicePointer(NodeName Name);

ExecutionResult COMM_NodeOutputReady(NodeName Name, uint16_t RegOutReady, bool *OutputReady);
ExecutionResult COMM_NodeCall(NodeName Name, uint16_t Command);

#endif // __COMMON_H

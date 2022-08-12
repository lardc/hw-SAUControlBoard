// Header
#include "Common.h"

// Includes
#include "BCCIMHighLevel.h"
#include "DataTable.h"

// Variables
//
static volatile SlaveNode NodeArray[MAX_SLAVE_NODES] = {0};

// Forward functions
//
void COMM_InitNode(NodeName Name, uint16_t RegNodeID, uint16_t RegEmulation);
void COMM_UpdateNodeEmulation(NodeName Name, uint16_t RegEmulation);
bool COMM_SlavesExecute(uint16_t Command);

// Functions
//
void COMM_InitSlaveArray()
{
	COMM_InitNode(NAME_Multiplexer,		REG_SLAVE_NID_MULTIPLEXER,	REG_EMULATE_MULTIPLEXER);
	COMM_InitNode(NAME_DCCurrent,		REG_SLAVE_NID_DC_CURRENT,	REG_EMULATE_DC_CURRENT);
	COMM_InitNode(NAME_DCHighVoltage,	REG_SLAVE_NID_DC_HV,		REG_EMULATE_DC_HV);
	COMM_InitNode(NAME_DCVoltage1,		REG_SLAVE_NID_DC_VOLTAGE1,	REG_EMULATE_DC_VOLTAGE1);
	COMM_InitNode(NAME_DCVoltage2,		REG_SLAVE_NID_DC_VOLTAGE2,	REG_EMULATE_DC_VOLTAGE2);
	COMM_InitNode(NAME_DCVoltage3,		REG_SLAVE_NID_DC_VOLTAGE3,	REG_EMULATE_DC_VOLTAGE3);
	COMM_InitNode(NAME_ACVoltage1,		REG_SLAVE_NID_AC_VOLTAGE1,	REG_EMULATE_AC_VOLTAGE1);
	COMM_InitNode(NAME_ACVoltage2,		REG_SLAVE_NID_AC_VOLTAGE2,	REG_EMULATE_AC_VOLTAGE2);
}
//-----------------------------

void COMM_UpdateEmulationSettings()
{
	COMM_UpdateNodeEmulation(NAME_Multiplexer,		REG_EMULATE_MULTIPLEXER);
	COMM_UpdateNodeEmulation(NAME_DCCurrent,		REG_EMULATE_DC_CURRENT);
	COMM_UpdateNodeEmulation(NAME_DCHighVoltage,	REG_EMULATE_DC_HV);
	COMM_UpdateNodeEmulation(NAME_DCVoltage1,		REG_EMULATE_DC_VOLTAGE1);
	COMM_UpdateNodeEmulation(NAME_DCVoltage2,		REG_EMULATE_DC_VOLTAGE2);
	COMM_UpdateNodeEmulation(NAME_DCVoltage3,		REG_EMULATE_DC_VOLTAGE3);
	COMM_UpdateNodeEmulation(NAME_ACVoltage1,		REG_EMULATE_AC_VOLTAGE1);
	COMM_UpdateNodeEmulation(NAME_ACVoltage2,		REG_EMULATE_AC_VOLTAGE2);
}
//-----------------------------

void COMM_InitNode(NodeName Name, uint16_t RegNodeID, uint16_t RegEmulation)
{
	NodeArray[Name].Emulation = DataTable[RegEmulation];
	NodeArray[Name].StateIsUpToDate = false;
	NodeArray[Name].NodeID = DataTable[RegNodeID];
	NodeArray[Name].Settings = NULL;
}
//-----------------------------

void COMM_UpdateNodeEmulation(NodeName Name, uint16_t RegEmulation)
{
	NodeArray[Name].Emulation = DataTable[RegEmulation];
}
//-----------------------------

bool COMM_SlavesExecute(uint16_t Command)
{
	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
	{
		if(!NodeArray[i].Emulation)
		{
			if(BHL_Call(NodeArray[i].NodeID, Command))
				NodeArray[i].StateIsUpToDate = false;
			else
				return false;
		}
	}

	return true;
}
//-----------------------------

bool COMM_SlavesReadState()
{
	uint16_t State, OpResult;
	bool result;

	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
	{
		if(!NodeArray[i].Emulation)
		{
			result = false;

			if(BHL_ReadRegister(NodeArray[i].NodeID, COMM_REG_DEV_STATE, &State))
				if(BHL_ReadRegister(NodeArray[i].NodeID, COMM_REG_OP_RESULT, &OpResult))
				{
					NodeArray[i].State = State;
					NodeArray[i].OpResult = OpResult;
					NodeArray[i].StateIsUpToDate = true;
					result = true;
				}

			if(!result)
				return false;
		}
		else
		{
			NodeArray[i].State = CDS_Ready;
			NodeArray[i].OpResult = COMM_OPRESULT_OK;
		}
	}

	return true;
}
//-----------------------------

bool COMM_AreSlavesInStateX(uint16_t State)
{
	bool result = true;

	for(uint16_t i = 0; i < MAX_SLAVE_NODES; ++i)
		if(!NodeArray[i].Emulation)
		{
			if(NodeArray[i].State != State || !NodeArray[i].StateIsUpToDate)
				result = false;
		}

	return result;
}
//-----------------------------

GetSlaveStateResult COMM_IsSlaveInStateX(NodeName Name, uint16_t State)
{
	if(NodeArray[Name].Emulation)
		return GSSR_Emulation;
	else if(!NodeArray[Name].StateIsUpToDate)
		return GSSR_Outdated;
	else if(NodeArray[Name].State == State)
		return GSSR_Equal;
	else
		return GSSR_NotEqual;
}
//-----------------------------

uint16_t COMM_GetSlaveOpResult(NodeName Name)
{
	return NodeArray[Name].OpResult;
}
//-----------------------------

bool COMM_IsSlaveInFaultOrDisabled()
{
	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
		if(!NodeArray[i].Emulation)
		{
			if((NodeArray[i].State == CDS_Fault || NodeArray[i].State == CDS_Disabled) && NodeArray[i].StateIsUpToDate)
				return true;
		}

	return false;
}
//-----------------------------

void COMM_ForceEmulationByNodeID(uint16_t NodeID)
{
	for(uint8_t i = 0; i < MAX_SLAVE_NODES; ++i)
		if(NodeArray[i].NodeID == NodeID)
			NodeArray[i].Emulation = true;
}
//-----------------------------

bool COMM_SlavesEnablePower()
{
	return COMM_SlavesExecute(COMM_ACT_ENABLE_POWER);
}
//-----------------------------

bool COMM_SlavesDisablePower()
{
	return COMM_SlavesExecute(COMM_ACT_DISABLE_POWER);
}
//-----------------------------

bool COMM_SlavesClearFault()
{
	return COMM_SlavesExecute(COMM_ACT_FAULT_CLEAR);
}
//-----------------------------

bool COMM_SlavesClearWarning()
{
	return COMM_SlavesExecute(COMM_ACT_WARNING_CLEAR);
}
//-----------------------------

pSlaveNode COMM_GetSlaveDevicePointer(NodeName Name)
{
	return (pSlaveNode)(&NodeArray[Name]);
}
//-----------------------------

ExecutionResult COMM_NodeOutputReady(NodeName Name, uint16_t RegOutReady, bool *OutputReady)
{
	*OutputReady = false;

	if(NodeArray[Name].Settings != NULL)
	{
		if(!NodeArray[Name].Emulation)
		{
			uint16_t NodeID = NodeArray[Name].NodeID;
			if(BHL_ReadRegister(NodeID, RegOutReady, (uint16_t *)OutputReady))
				return ER_NoError;
		}
		else
		{
			*OutputReady = true;
			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult COMM_NodeCall(NodeName Name, uint16_t Command)
{
	if(NodeArray[Name].Settings != NULL)
	{
		if(!NodeArray[Name].Emulation)
		{
			uint16_t NodeID = NodeArray[Name].NodeID;
			if(BHL_Call(NodeID, Command))
			{
				NodeArray[Name].StateIsUpToDate = false;
				return ER_NoError;
			}
		}
		else
			return ER_NoError;

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

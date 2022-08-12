// Header
#include "DCHighVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult DCHV_Execute()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
	pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint32_t Current = Settings->Setpoint.Current * 100;
			uint32_t Voltage = Settings->Setpoint.Voltage / 1e5;

			uint16_t VoltageLow = (uint16_t)Voltage;
			uint16_t CurrentLow = (uint16_t)(Current & 0xFFFF);
			uint16_t CurrentHigh = (uint16_t)(Current >> 16);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, DCHV_REG_CURRENT_SETPOINT, CurrentLow))
				if(BHL_WriteRegister(NodeID, DCHV_REG_CURRENT_SETPOINT_32, CurrentHigh))
					if(BHL_WriteRegister(NodeID, DCHV_REG_VOLTAGE_SETPOINT, VoltageLow))
						if(BHL_Call(NodeID, DCHV_ACT_START_PROCESS))
						{
							NodeData->StateIsUpToDate = false;
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

ExecutionResult DCHV_ReadResult()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCHighVoltage);
	pDCHVoltageBoardObject Settings = (pDCHVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, DCHV_REG_CURRENT_RESULT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, DCHV_REG_CURRENT_RESULT_32, &CurrentHigh))
					if(BHL_ReadRegister(NodeID, DCHV_REG_VOLTAGE_RESULT, &VoltageLow))
					{
						uint32_t Current;
						Current = CurrentLow;
						Current |= (uint32_t)CurrentHigh << 16;

						Settings->Result.Current = Current / 100;
						Settings->Result.Voltage = (uint32_t)VoltageLow * 1e5;
						return ER_NoError;
					}
		}
		else
		{
			Settings->Result.Current = DCHV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = DCHV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult DCHV_ExecuteNext()
{
	return COMM_NodeCall(NAME_DCHighVoltage, DCHV_ACT_SECOND_START_PROCESS);
}
//-----------------------------

ExecutionResult DCHV_Stop()
{
	return COMM_NodeCall(NAME_DCHighVoltage, DCHV_ACT_STOP_PROCESS);
}
//-----------------------------

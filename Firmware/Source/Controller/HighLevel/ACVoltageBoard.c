// Header
#include "ACVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult ACV_Execute(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint32_t Voltage = Settings->Setpoint.Voltage / 1000;
			uint32_t Current = Settings->Setpoint.Current;

			uint16_t VoltageLow = (uint16_t)(Voltage & 0xFFFF);
			uint16_t VoltageHigh = (uint16_t)(Voltage >> 16);
			uint16_t CurrentLow = (uint16_t)(Current & 0xFFFF);
			uint16_t CurrentHigh = (uint16_t)(Current >> 16);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, ACV_REG_OUTPUT_LINE, Settings->OutputLine))
				if(BHL_WriteRegister(NodeID, ACV_REG_VOLTAGE_SETPOINT, VoltageLow))
					if(BHL_WriteRegister(NodeID, ACV_REG_VOLTAGE_SETPOINT_32, VoltageHigh))
						if(BHL_WriteRegister(NodeID, ACV_REG_CURRENT_SETPOINT, CurrentLow))
							if(BHL_WriteRegister(NodeID, ACV_REG_CURRENT_SETPOINT_32, CurrentHigh))
							{
								if(BHL_Call(NodeID, ACV_ACT_START_PROCESS))
								{
									NodeData->StateIsUpToDate = false;
									return ER_NoError;
								}
								else
								{
									BHLError err = BHL_GetError();
									if(err.Func == FUNCTION_CALL && err.ErrorCode == ERR_USER &&
											err.ExtData == ACV_ACT_START_PROCESS && err.Details == COMM_ERR_BAD_CONFIG)
									{
										return ER_BadHighLevelConfig;
									}
								}
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

ExecutionResult ACV_ReadResult(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pACVoltageBoardObject Settings = (pACVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0, VoltageHigh = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, ACV_REG_VOLTAGE_RESULT, &VoltageLow))
				if(BHL_ReadRegister(NodeID, ACV_REG_VOLTAGE_RESULT_32, &VoltageHigh))
					if(BHL_ReadRegister(NodeID, ACV_REG_CURRENT_RESULT, &CurrentLow))
						if(BHL_ReadRegister(NodeID, ACV_REG_CURRENT_RESULT_32, &CurrentHigh))
						{
							uint32_t Voltage = VoltageLow;
							Voltage |= (uint32_t)VoltageHigh << 16;

							uint32_t Current = CurrentLow;
							Current |= (uint32_t)CurrentHigh << 16;

							Settings->Result.Voltage = Voltage * 1000;
							Settings->Result.Current = Current;
							return ER_NoError;
						}
		}
		else
		{
			Settings->Result.Current = ACV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = ACV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult ACV_Stop(NodeName Name)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	return COMM_NodeCall(Name, ACV_ACT_STOP_PROCESS);
}
//-----------------------------

ExecutionResult ACV_IsVoltageReady(NodeName Name, bool *VoltageReady)
{
	if(Name != NAME_ACVoltage1 && Name != NAME_ACVoltage2)
		return ER_WrongNode;

	return COMM_NodeOutputReady(Name, ACV_REG_VOLTAGE_READY, VoltageReady);
}
//-----------------------------

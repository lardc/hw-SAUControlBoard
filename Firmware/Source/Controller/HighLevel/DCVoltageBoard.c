// Header
#include "DCVoltageBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult DCV_Execute(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

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

			if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_LINE, Settings->OutputLine))
				if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_TYPE, Settings->OutputType))
					if(BHL_WriteRegister(NodeID, DCV_REG_OUTPUT_MODE, Settings->OutputMode))
						if(BHL_WriteRegister(NodeID, DCV_REG_PULSE_LENGTH, Settings->PulseLength))
							if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT, CurrentLow))
								if(BHL_WriteRegister(NodeID, DCV_REG_CURRENT_SETPOINT_32, CurrentHigh))
									if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT, VoltageLow))
										if(BHL_WriteRegister(NodeID, DCV_REG_VOLTAGE_SETPOINT_32, VoltageHigh))
										{
											if(BHL_Call(NodeID, DCV_ACT_START_PROCESS))
											{
												NodeData->StateIsUpToDate = false;
												return ER_NoError;
											}
											else
											{
												BHLError err = BHL_GetError();
												if(err.Func == FUNCTION_CALL && err.ErrorCode == ERR_USER &&
														err.ExtData == DCV_ACT_START_PROCESS && err.Details == COMM_ERR_BAD_CONFIG)
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

ExecutionResult DCV_ReadResult(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(Name);
	pDCVoltageBoardObject Settings = (pDCVoltageBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0, VoltageHigh = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, DCV_REG_CURRENT_RESULT_32, &CurrentHigh))
					if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT, &VoltageLow))
						if(BHL_ReadRegister(NodeID, DCV_REG_VOLTAGE_RESULT_32, &VoltageHigh))
						{
							uint32_t Voltage = VoltageLow;
							Voltage |= (uint32_t)VoltageHigh << 16;

							uint32_t Current = CurrentLow;
							Current |= (uint32_t)CurrentHigh << 16;

							Settings->Result.Voltage = Voltage * 1000;
							Settings->Result.Current = Current / 100;
							return ER_NoError;
						}
		}
		else
		{
			Settings->Result.Current = DCV_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = DCV_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult DCV_Stop(NodeName Name)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	return COMM_NodeCall(Name, DCV_ACT_STOP_PROCESS);
}
//-----------------------------

ExecutionResult DCV_IsVoltageReady(NodeName Name, bool *VoltageReady)
{
	if(Name != NAME_DCVoltage1 && Name != NAME_DCVoltage2 && Name != NAME_DCVoltage3)
		return ER_WrongNode;

	return COMM_NodeOutputReady(Name, DCV_REG_VOLTAGE_READY, VoltageReady);
}
//-----------------------------

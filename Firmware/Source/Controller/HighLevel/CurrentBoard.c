// Header
#include "CurrentBoard.h"

// Includes
#include "BCCIMHighLevel.h"

// Functions
//
ExecutionResult CURR_Execute()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
	pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = (uint16_t)(Settings->Setpoint.Current & 0xFFFF);
			uint16_t CurrentHigh = (uint16_t)(Settings->Setpoint.Current >> 16);
			uint16_t VoltageLow = (uint16_t)(Settings->Setpoint.Voltage & 0xFFFF);
			uint16_t VoltageHigh = (uint16_t)(Settings->Setpoint.Voltage >> 16);
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT, CurrentLow))
				if(BHL_WriteRegister(NodeID, CURR_REG_CURRENT_SETPOINT_32, CurrentHigh))
					if(BHL_WriteRegister(NodeID, CURR_REG_VOLTAGE_DUT_LIM, VoltageLow))
						if(BHL_WriteRegister(NodeID, CURR_REG_VOLTAGE_DUT_LIM_32, VoltageHigh))
						{
							if(BHL_Call(NodeID, CURR_ACT_START_PROCESS))
							{
								NodeData->StateIsUpToDate = false;
								return ER_NoError;
							}
							else
							{
								BHLError err = BHL_GetError();
								if(err.Func == FUNCTION_CALL && err.ErrorCode == ERR_USER &&
										err.ExtData == CURR_ACT_START_PROCESS && err.Details == COMM_ERR_BAD_CONFIG)
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

ExecutionResult CURR_ReadResult()
{
	pSlaveNode NodeData = COMM_GetSlaveDevicePointer(NAME_DCCurrent);
	pCurrentBoardObject Settings = (pCurrentBoardObject)NodeData->Settings;

	if(Settings != NULL)
	{
		if(!NodeData->Emulation)
		{
			uint16_t CurrentLow = 0, CurrentHigh = 0, VoltageLow = 0, VoltageHigh = 0;
			uint16_t NodeID = NodeData->NodeID;

			if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT, &CurrentLow))
				if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_CURRENT_32, &CurrentHigh))
					if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_VOLTAGE, &VoltageLow))
						if(BHL_ReadRegister(NodeID, CURR_REG_RESULT_VOLTAGE_32, &VoltageHigh))
						{
							Settings->Result.Current = CurrentLow;
							Settings->Result.Current |= (uint32_t)CurrentHigh << 16;
							Settings->Result.Voltage = VoltageLow;
							Settings->Result.Voltage |= (uint32_t)VoltageHigh << 16;
							return ER_NoError;
						}
		}
		else
		{
			Settings->Result.Current = CURR_EMULATION_RES_CURRENT;
			Settings->Result.Voltage = CURR_EMULATION_RES_VOLTAGE;

			return ER_NoError;
		}

		return ER_InterfaceError;
	}
	else
		return ER_SettingsError;
}
//-----------------------------

ExecutionResult CURR_Stop()
{
	return COMM_NodeCall(NAME_DCCurrent, CURR_ACT_STOP_PROCESS);
}
//-----------------------------

ExecutionResult CURR_AfterPulseReady(bool *Ready)
{
	bool Register;
	ExecutionResult res = COMM_NodeOutputReady(NAME_DCCurrent, CURR_REG_AFTER_PULSE_DELAY, &Register);
	*Ready = !Register;

	return res;
}
//-----------------------------

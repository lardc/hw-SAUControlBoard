// Header
#include "Diagnostic.h"

// Includes
#include "DebugActions.h"
#include "DeviceObjectDictionary.h"

// Functions
bool DIAG_HandleDiagnosticAction(Int16U ActionID, Int16U *pUserError)
{
	switch (ActionID)
	{
		case ACT_DIAG_INT_FAN:
			DBGACT_INT_FAN();
		break;

		case ACT_DIAG_GREEN_LED:
			DBGACT_GREEN_LED();
		break;

		case ACT_DIAG_RED_LED:
			DBGACT_RED_LED();
		break;

		case ACT_DIAG_PC_SWITCH:
			DBGACT_PC_SWITCH();
		break;



		default:
			return false;
	}

	return true;
}

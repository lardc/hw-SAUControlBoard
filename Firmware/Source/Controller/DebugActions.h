#ifndef __DEBUGACTIONS_H
#define __DEBUGACTIONS_H

// Include
//
#include "ZwBase.h"

// Functions
//
void DBGACT_Relay();
void DBGACT_Switch();
void DBGACT_LampGreen();
void DBGACT_LampRed();
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError);

#endif //__DEBUGACTIONS_H

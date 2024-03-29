#ifndef CONTROLLER_SELFTEST_H_
#define CONTROLLER_SELFTEST_H_

// Types
//
typedef enum __DeviceSelfTestStage
{
	STS_None 			= 0,
	STS_OptBarier		= 1,
	STS_Door			= 2,
	STS_Input1			= 3,
	STS_Input2			= 4,
	STS_STOP			= 5,
	STS_Delay			= 6,
	STS_CheckOutputs	= 7,
	STS_Finish			= 8
} DeviceSelfTestStage;

// Variables
//
extern volatile DeviceSelfTestStage SELFTEST_Stage;

// Functions
//
void SELFTEST_Process();
void SELFTTEST_SetStage(DeviceSelfTestStage NewStage);

#endif /* CONTROLLER_SELFTEST_H_ */

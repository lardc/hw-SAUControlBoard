// ----------------------------------------
// Device profile
// ----------------------------------------

// Header
#include "DeviceProfile.h"
//
// Includes
#include "SysConfig.h"
#include "Global.h"
#include "DataTable.h"
#include "Controller.h"
#include "Constraints.h"
#include "ZwNCAN.h"
#include "ZwUSART.h"

// Types
//
typedef struct __FEPState
{
	Int16U Size;
	Int16U ReadCounter;
	Int16U WriteCounter;
	Int16U LastReadCounter;
	pInt16U pDataCounter;
	float* Data;
} FEPState, *pFEPState;
//
typedef struct __EPState
{
	Int16U Size;
	Int16U ReadCounter;
	Int16U WriteCounter;
	Int16U LastReadCounter;
	pInt16U pDataCounter;
	pInt16U Data;
} EPState, *pEPState;
//
typedef struct __EPStates
{
	EPState EPs[EP_COUNT];
	EPState WriteEPs[EP_WRITE_COUNT];
	FEPState FEPs[FEP_COUNT];
} EPStates, *pEPStates;

// Variables
//
SCCI_Interface DEVICE_USB_UART1_Interface, DEVICE_USB_UART3_Interface;
BCCI_Interface DEVICE_CAN_Interface;
BCCIM_Interface MASTER_DEVICE_CAN_Interface;
//
static SCCI_IOConfig USB_UART1_IOConfig, USB_UART3_IOConfig;
static BCCI_IOConfig CAN_IOConfig;
static xCCI_ServiceConfig X_ServiceConfig;
static EPStates DummyEPState;
static xCCI_FUNC_CallbackAction ControllerDispatchFunction;
//
static Boolean* MaskChangesFlag;

// Forward functions
//
static Boolean DEVPROFILE_Validate16(Int16U Address, Int16U Data);
static Boolean DEVPROFILE_ValidateFloat(Int16U Address, float Data, float* LowLimit, float* HighLimit);
static Boolean DEVPROFILE_DispatchAction(Int16U ActionID, pInt16U UserError);
static void DEVPROFILE_FillWRPartDefault();

// Functions
//
void DEVPROFILE_Init(xCCI_FUNC_CallbackAction SpecializedDispatch, Boolean* MaskChanges)
{
	// Save values
	ControllerDispatchFunction = SpecializedDispatch;
	MaskChangesFlag = MaskChanges;
	
	// Init interface
	USB_UART1_IOConfig.IO_SendArray16 = &USART1_SendArray16;
	USB_UART1_IOConfig.IO_ReceiveArray16 = &USART1_ReceiveArray16;
	USB_UART1_IOConfig.IO_GetBytesToReceive = &USART1_GetBytesToReceive;
	USB_UART1_IOConfig.IO_ReceiveByte = &USART1_ReceiveChar;

	USB_UART3_IOConfig.IO_SendArray16 = &USART3_SendArray16;
	USB_UART3_IOConfig.IO_ReceiveArray16 = &USART3_ReceiveArray16;
	USB_UART3_IOConfig.IO_GetBytesToReceive = &USART3_GetBytesToReceive;
	USB_UART3_IOConfig.IO_ReceiveByte = &USART3_ReceiveChar;

	CAN_IOConfig.IO_SendMessage = &NCAN_SendMessage;
	CAN_IOConfig.IO_SendMessageEx = &NCAN_SendMessageEx;
	CAN_IOConfig.IO_GetMessage = &NCAN_GetMessage;
	CAN_IOConfig.IO_IsMessageReceived = &NCAN_IsMessageReceived;
	CAN_IOConfig.IO_ConfigMailbox = &NCAN_ConfigMailbox;
	
	// Init service
	X_ServiceConfig.UserActionCallback = &DEVPROFILE_DispatchAction;
	X_ServiceConfig.ValidateCallback16 = &DEVPROFILE_Validate16;
	X_ServiceConfig.ValidateCallbackFloat = &DEVPROFILE_ValidateFloat;
	
	// Init interface driver
	SCCI_Init(&DEVICE_USB_UART1_Interface, &USB_UART1_IOConfig, &X_ServiceConfig, (pInt16U)DataTable, DATA_TABLE_SIZE,
			xCCI_TIMEOUT_TICKS, &DummyEPState);
	SCCI_Init(&DEVICE_USB_UART3_Interface, &USB_UART3_IOConfig, &X_ServiceConfig, (pInt16U)DataTable, DATA_TABLE_SIZE,
			xCCI_TIMEOUT_TICKS, &DummyEPState);
	BCCI_Init(&DEVICE_CAN_Interface, &CAN_IOConfig, &X_ServiceConfig, (pInt16U)DataTable, DATA_TABLE_SIZE, &DummyEPState);
	BCCIM_Init(&MASTER_DEVICE_CAN_Interface, &CAN_IOConfig, xCCI_TIMEOUT_TICKS, &CONTROL_TimeCounter);

	// Set write protection
	SCCI_AddProtectedArea(&DEVICE_USB_UART1_Interface, DATA_TABLE_WP_START, DATA_TABLE_SIZE - 1);
	SCCI_AddProtectedArea(&DEVICE_USB_UART3_Interface, DATA_TABLE_WP_START, DATA_TABLE_SIZE - 1);
	BCCI_AddProtectedArea(&DEVICE_CAN_Interface, DATA_TABLE_WP_START, DATA_TABLE_SIZE - 1);
}
// ----------------------------------------

void DEVPROFILE_ProcessRequests()
{
	// Handle interface requests
	SCCI_Process(&DEVICE_USB_UART1_Interface, CONTROL_TimeCounter, *MaskChangesFlag);
	SCCI_Process(&DEVICE_USB_UART3_Interface, CONTROL_TimeCounter, *MaskChangesFlag);

	// Handle interface requests
	BCCI_Process(&DEVICE_CAN_Interface, *MaskChangesFlag);
}
// ----------------------------------------

void DEVPROFILE_ResetControlSection()
{
	DT_ResetWRPart(&DEVPROFILE_FillWRPartDefault);
}
// ----------------------------------------

static void DEVPROFILE_FillWRPartDefault()
{
	for(Int16U i = DATA_TABLE_WR_START; i < DATA_TABLE_WP_START; ++i)
		DataTable[i] = Constraint[i].Default;
}
// ----------------------------------------

void DEVPROFILE_FillNVPartDefault(void)
{
	for(Int16U i = 0; i < DATA_TABLE_NV_SIZE; ++i)
		DataTable[i] = Constraint[i].Default;
}
// ----------------------------------------

static Boolean DEVPROFILE_Validate16(Int16U Address, Int16U Data)
{
	if(Address < DATA_TABLE_WP_START)
		return (Constraint[Address].Min <= Data) && (Data <= Constraint[Address].Max);
	else
		return FALSE;
}
// ----------------------------------------

static Boolean DEVPROFILE_ValidateFloat(Int16U Address, float Data, float* LowLimit, float* HighLimit)
{
	if(LowLimit && HighLimit)
	{
		*LowLimit = Constraint[Address].Min;
		*HighLimit = Constraint[Address].Max;

		return TRUE;
	}
	else if(Address < DATA_TABLE_WP_START)
		return (Constraint[Address].Min <= Data) && (Data <= Constraint[Address].Max);
	else
		return FALSE;
}
// ----------------------------------------

static Boolean DEVPROFILE_DispatchAction(Int16U ActionID, pInt16U UserError)
{
	switch (ActionID)
	{
		case ACT_SAVE_TO_ROM:
			DT_SaveNVPartToEPROM();
			break;

		case ACT_RESTORE_FROM_ROM:
			DT_RestoreNVPartFromEPROM();
			break;

		case ACT_RESET_TO_DEFAULT:
			DT_ResetNVPart(&DEVPROFILE_FillNVPartDefault);
			break;

		case ACT_BOOT_LOADER_REQUEST:
			BOOT_LOADER_VARIABLE = BOOT_LOADER_REQUEST;
			break;

		default:
			return (ControllerDispatchFunction) ? ControllerDispatchFunction(ActionID, UserError) : FALSE;
	}
	
	return TRUE;
}
// ----------------------------------------

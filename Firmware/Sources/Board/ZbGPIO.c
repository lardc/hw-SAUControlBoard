// -----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

// Header
#include "ZbGPIO.h"
//
#include "SysConfig.h"

#define REFdebounce	50

Int16U 	sCounter1_0 = 0, sCounter1_1 = 0,
		sCounter2_0 = 0, sCounter2_1 = 0,
		sCounter3_0 = 0, sCounter3_1 = 0,
		sCounter4_0 = 0, sCounter4_1 = 0;

Boolean Sensor1 = FALSE, Sensor2 = FALSE, Sensor3 = FALSE, Sensor4 = FALSE;

// Functions
//
void ZbGPIO_Init()
{
   	ZwGPIO_WritePin(PIN_LED, FALSE);
   	ZwGPIO_WritePin(PIN_OPTO_SW, FALSE);
   	ZwGPIO_PinToOutput(PIN_LED);
   	ZwGPIO_PinToOutput(PIN_OPTO_SW);

   	ZwGPIO_WritePin(PIN_LAMP_0, FALSE);
   	ZwGPIO_WritePin(PIN_LAMP_1, FALSE);
   	ZwGPIO_WritePin(PIN_LAMP_2, FALSE);
   	ZwGPIO_PinToOutput(PIN_LAMP_0);
   	ZwGPIO_PinToOutput(PIN_LAMP_1);
   	ZwGPIO_PinToOutput(PIN_LAMP_2);

   	ZwGPIO_PinToInput(PIN_SENS_0, TRUE, 0);
   	ZwGPIO_PinToInput(PIN_SENS_1, TRUE, 0);
   	ZwGPIO_PinToInput(PIN_SENS_2, TRUE, 0);
   	ZwGPIO_PinToInput(PIN_SENS_3, TRUE, 0);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZbGPIO_SwitchLedPin, "ramfuncs");
#endif
void ZbGPIO_SwitchLedPin(Boolean Set)
{
	ZwGPIO_WritePin(PIN_LED, Set);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZbGPIO_ToggleLedPin, "ramfuncs");
#endif
void ZbGPIO_ToggleLedPin()
{
	ZwGPIO_TogglePin(PIN_LED);
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZbGPIO_SwitchLampPin, "ramfuncs");
#endif
void ZbGPIO_SwitchLampPin(Lamps Lamp, Boolean Set)
{
	switch(Lamp)
	{
		case Lamp_1:
			ZwGPIO_WritePin(PIN_LAMP_0, Set);
			break;
		case Lamp_2:
			ZwGPIO_WritePin(PIN_LAMP_1, Set);
			break;
		case Lamp_3:
			ZwGPIO_WritePin(PIN_LAMP_2, Set);
			break;
	}
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZbGPIO_ToggleLampPin, "ramfuncs");
#endif
void ZbGPIO_ToggleLampPin(Lamps Lamp)
{
	switch(Lamp)
	{
		case Lamp_1:
			ZwGPIO_TogglePin(PIN_LAMP_0);
			break;
		case Lamp_2:
			ZwGPIO_TogglePin(PIN_LAMP_1);
			break;
		case Lamp_3:
			ZwGPIO_TogglePin(PIN_LAMP_2);
			break;
	}
}
// ----------------------------------------

#ifdef BOOT_FROM_FLASH
	#pragma CODE_SECTION(ZbGPIO_ReadSensors, "ramfuncs");
#endif
void ZbGPIO_ReadSensors()
{
	if (ZwGPIO_ReadPin(PIN_SENS_0) == 0)
	{
		sCounter1_0 ++;
		sCounter1_1 = 0;

		if (sCounter1_0 >= REFdebounce)
		{
			sCounter1_0 = REFdebounce + 1;
			Sensor1 = TRUE;
		}
	}
	else
	{
		sCounter1_0 = 0;
		sCounter1_1 ++;

		if (sCounter1_1 >= REFdebounce)
		{
			sCounter1_1 = REFdebounce + 1;
			Sensor1 = FALSE;
		}
	}

	//------

	if (ZwGPIO_ReadPin(PIN_SENS_1) == 0)
	{
		sCounter2_0 ++;
		sCounter2_1 = 0;

		if (sCounter2_0 >= REFdebounce)
		{
			sCounter2_0 = REFdebounce + 1;
			Sensor2 = TRUE;
		}
	}
	else
	{
		sCounter2_0 = 0;
		sCounter2_1 ++;

		if (sCounter2_1 >= REFdebounce)
		{
			sCounter2_1 = REFdebounce + 1;
			Sensor2 = FALSE;
		}
	}

	//------

	if (ZwGPIO_ReadPin(PIN_SENS_2) == 0)
	{
		sCounter3_0 ++;
		sCounter3_1 = 0;

		if (sCounter3_0 >= REFdebounce)
		{
			sCounter3_0 = REFdebounce + 1;
			Sensor3 = TRUE;
		}
	}
	else
	{
		sCounter3_0 = 0;
		sCounter3_1 ++;

		if (sCounter3_1 >= REFdebounce)
		{
			sCounter3_1 = REFdebounce + 1;
			Sensor3 = FALSE;
		}
	}

	//------

	if (ZwGPIO_ReadPin(PIN_SENS_3) == 0)
	{
		sCounter4_0 ++;
		sCounter4_1 = 0;

		if (sCounter4_0 >= REFdebounce)
		{
			sCounter4_0 = REFdebounce + 1;
			Sensor4 = TRUE;
		}
	}
	else
	{
		sCounter4_0 = 0;
		sCounter4_1 ++;

		if (sCounter4_1 >= REFdebounce)
		{
			sCounter4_1 = REFdebounce + 1;
			Sensor4 = FALSE;
		}
	}
}

// No more.

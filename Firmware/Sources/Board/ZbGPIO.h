// -----------------------------------------
// Board-specific GPIO functions
// ----------------------------------------

#ifndef __ZBGPIO_H
#define __ZBGPIO_H

// Include
#include "stdinc.h"
#include "ZwDSP.h"

// Types
//

typedef enum __Lamps
{
	Lamp_None		= 0,
	Lamp_1			= 1,
	Lamp_2			= 2,
	Lamp_3			= 3,
	Lamp_4			= 4
} Lamps;

extern Boolean Sensor1;
extern Boolean Sensor2;
extern Boolean Sensor3;
extern Boolean Sensor4;

// Functions
//
// Init GPIO and GPIO-PWM
void ZbGPIO_Init();
// Switch state of led pin
void ZbGPIO_SwitchLedPin(Boolean Set);
// Toggle led pin
void ZbGPIO_ToggleLedPin();
// Switch state of lamp pin
void ZbGPIO_SwitchLampPin(Lamps Lamp, Boolean Set);
// Toggle lamp pin
void ZbGPIO_ToggleLampPin(Lamps Lamp);
// Get pressed sensor
void ZbGPIO_ReadSensors();

#endif // __ZBGPIO_H

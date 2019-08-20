///////////////////////////////////////////////////////////////////////////
/// \file OvenControl.cpp
/// \brief Handles controller the temperature of the oven. this will also control the cooling fan
/// \author Ronald Sousa
///////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "configuration.h"

///////////////////////////////////////////////////////////////////////////
/// \brief Defines the PID parameter for the heater
///////////////////////////////////////////////////////////////////////////
PIDType HeaterPID = {HeaterPID_P, HeaterPID_I, HeaterPID_D, 0, 0};

static float TargetTemperature = 0;
static float PWMOutput = 0;

///////////////////////////////////////////////////////////////////////////
/// \brief Configures the hardware to
///////////////////////////////////////////////////////////////////////////
void OvenControl_Init(void)
{
	Temperature_Init();
	PID_Init();
	PWM_Init(1, 0); // set to 1hz and set motors to off

}

void OvenControl_Process(float *currentTemp, float *currentTagetTemp, float *heaterPWM, uint32_t disableAll)
{
	float temperature = 0;

	Temperature_Read(&temperature, 0, 0);
	PWMOutput =  PID_Process(&HeaterPID, TargetTemperature, temperature);

	if( disableAll )
	{
		PWM_SetDuty(PWMPin1, 0);
		PWM_SetDuty(PWMPin2, 0);
	}
	else
	{
		PWM_SetDuty(PWMPin1, PWMOutput);
	}

	if ( currentTemp )
	{
		*currentTemp = temperature;
	}

	if ( currentTagetTemp )
	{
		*currentTagetTemp = TargetTemperature;
	}

	if ( heaterPWM )
	{
		*heaterPWM = PWMOutput;
	}

}

void OvenControl_SetTemperature(float temperature)
{
	TargetTemperature = temperature;
}




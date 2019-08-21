///////////////////////////////////////////////////////////////////////////
/// \file OvenControl.cpp
/// \brief Handles controller the temperature of the oven. this will also control the cooling fan
/// \author Ronald Sousa
///////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "OvenControl.h"
#include "configuration.h"


/**
 * Defines the PID parameter for the heater
 */
PIDType HeaterPID = {HeaterPID_P, HeaterPID_I, HeaterPID_D, 0, 0};

static float Temperature = 0;
static float TargetTemperature = 0;
static float PWMOutput = 0;
static SolderProfileInterface * ProfilePointer = NULL;

/**
 * defines the internal state of the over
 */
typedef enum  {
	OverControlState_Idel,
	OverControlState_Running,
} OverControlStateEnum;

static OverControlStateEnum OverControlState = OverControlState_Idel;

/**
 * Set the solder profile. If an invalid pointer is set, then it will reset our local one to null
 * @param profile is the pointer to the solder profile to set
 */
void OvenControl_SetProfile(SolderProfileInterface * profile) {

	if(profile) {
		ProfilePointer = profile;
	} else {
		// remove the last pointer value just in case
		ProfilePointer = NULL;
	}

}

/**
 * Stop and reset the reflow over state
 */
void OvenControl_Stop(void) {
	OverControlState = OverControlState_Idel;
	OvenControl_SetTemperature(0);
	PWM_SetDuty(HeaterPwmPin, 0);
	PWM_SetDuty(MoterPwmPin, 0);
}
/**
 * This is the ain over control process
 */
void OvenControl_Process(float *currentTemp, float *currentTagetTemp, float *heaterPWM) {

	Temperature_Read(&Temperature, 0, 0);
	PWMOutput =  PID_Process(&HeaterPID, TargetTemperature, Temperature);
	PWM_SetDuty(HeaterPwmPin, PWMOutput);

	if ( currentTemp ) {
		*currentTemp = Temperature;
	}

	if ( currentTagetTemp ) {
		*currentTagetTemp = TargetTemperature;
	}

	if ( heaterPWM ){
		*heaterPWM = PWMOutput;
	}

}

/**
 * this function can be used to set the Reflow running using the last set profile
 */
OverControlStatusEnum OverControl_Run(void) {

	if(!ProfilePointer) {
		return OverControlStatus_InvalidPointer;
	}

	// Check if we are ideling and have a valid profile pointer
	if(OverControlState_Idel == OverControlState) {

		if(ProfilePointer->MaxStartTemperature <= Temperature) {
			// we can start
			OverControlState = OverControlState_Running;
			return OverControlStatus_Ok;
		} else {
			return OverControlStatus_TooHot;
		}

	}

	return OverControlStatus_InvalidPointer;
}


/**
 * manually set the temperature
 * 
 * @param temperature
 */
OverControlStatusEnum OvenControl_SetTemperature(float temperature) {
	if(OverControlState_Idel == OverControlState) {
		TargetTemperature = temperature;
		return OverControlStatus_Ok;
	}

	return OverControlStatus_Error;
}


/**
 * Configures the hardware
 * 
 * @param profile is the pointer to the solder profile to set
 */
void OvenControl_Init(SolderProfileInterface * profile){

	Temperature_Init();
	PID_Init();
	PWM_Init(1, 0); // set to 1hz and set motors to off
	OvenControl_SetProfile(profile);

	OverControlState = OverControlState_Idel;
}
///////////////////////////////////////////////////////////////////////////
/// \file OvenControl.cpp
/// \brief Handles controller the temperature of the oven. this will also control the cooling fan
/// \author Ronald Sousa
///////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "OvenControl.h"
#include "./Terminal/Terminal.h"
#include "configuration.h"

/**
 * this is how much the reflow has to be hoter than the target temperature before the motor comes on
 */
static const uint_fast8_t MotorTemperatureTriggerThreshold = 15;
/**
 * defines the internal state of the over
 */
typedef enum  {
	OverControlState_Idel,
	OverControlState_Running,
} OverControlStateEnum;

/**
 * defines the various internal data required by the oven controller
 */
typedef struct {
	OverControlStateEnum state;
	TickType timer;
	SolderProfileInterface * profile; ///< selected profile
	uint32_t profileStep; ///< Keep track the current profile state
	uint32_t profileDuration; ///< Keep of the current step duration
	uint32_t timeLapse; ///< This is how long this current profile has been running 
	float temperature;
	float targetTemperature;
	float heaterPwm;
	float motorPwm;
	PIDType heaterPID; 	///< Defines the PID parameter for the heater

} OvenParameterType;

/**
 * holes the Over internal data
 */
static OvenParameterType OvenParameters = {
	.state = OverControlState_Idel,
	.timer  = { 0 , 1000},
	.profile = NULL,
	.profileStep = 0,
	.profileDuration = 0,
	.timeLapse = 0,
	.temperature = 0,
	.targetTemperature = 0,
	.heaterPwm = 0,
	.motorPwm = 0,
	.heaterPID = {HeaterPID_P, HeaterPID_I, HeaterPID_D, 0, 0},

	
};


/**
 * Set the solder profile. If an invalid pointer is set, then it will reset our local one to null
 * @param profile is the pointer to the solder profile to set
 */
void OvenControl_SetProfile(SolderProfileInterface * profile) {

	if(profile) {
		OvenParameters.profile = profile;
		OvenParameters.profileStep = 0;
		OvenParameters.profileDuration = 0;
		OvenParameters.timeLapse = 0;
	} else {
		// remove the last pointer value just in case
		OvenParameters.profile = NULL;
	}

}

/**
 * Stop and reset the reflow over state
 */
void OvenControl_Stop(void) {
	OvenParameters.state = OverControlState_Idel;

	OvenControl_SetTemperature(0);
	OverControl_SetFanPower(0);
	PWM_SetDuty(HeaterPwmPin, 0);
	PWM_SetDuty(MotorPwmPin, 0);
	
	Terminal_WriteString((uint8_t *)"Oven Stop\n\r");
}


/**
 * This is the ain over control process
 */
void OvenControl_Process(uint8_t *character) {
uint8_t MessageBuffer[100];
	// check if we need to update the screen
	if (Tick_DelayMs_NonBlocking(FALSE, &OvenParameters.timer)) {
			Tick_DelayMs_NonBlocking(TRUE, &OvenParameters.timer); // reset the tick counter

			Temperature_Read(&OvenParameters.temperature, 0, 0);

			if (OvenParameters.state == OverControlState_Running) {
				// we are running the profile sequence
				if(!OvenParameters.profileDuration) {
					// Do we still data point to run through
					if (OvenParameters.profileStep >= OvenParameters.profile->NumberOfDataPoints) {
						Terminal_WriteString((uint8_t *)"Reflow Complete\n\r");
						return OvenControl_Stop();
					}
					Terminal_WriteString((uint8_t *)"temperatur set\n\r");
					OvenParameters.profileDuration = 10;//OvenParameters.profile->points[OvenParameters.profileStep]->seconds;
															Terminal_WriteString((uint8_t *)"temperatur set\n\r");
					//OvenControl_SetTemperature((float)OvenParameters.profile->points[OvenParameters.profileStep]->temperature);
					OvenControl_SetTemperature(OvenParameters.profile->points[OvenParameters.profileStep].temperature);
					OvenParameters.profileStep++;
				} else {
					OvenParameters.profileDuration--;
					Terminal_WriteString((uint8_t *)"Duration\n\r");
				}

				if (OvenParameters.temperature > (OvenParameters.targetTemperature + MotorTemperatureTriggerThreshold)) {
					OverControl_SetFanPower(100);
				} else {
					OverControl_SetFanPower(0);
				}
				OvenParameters.timeLapse ++;
				Terminal_WriteString((uint8_t *)"lapse\n\r");
			}
			

			OvenParameters.heaterPwm =  PID_Process(&OvenParameters.heaterPID, OvenParameters.targetTemperature, OvenParameters.temperature);
			PWM_SetDuty(HeaterPwmPin, OvenParameters.heaterPwm);
			
	}
}

/**
 * this function can be used to set the Reflow running using the last set profile
 */
OverControlStatusEnum OverControl_Start(void) {

	if(!OvenParameters.profile) {
		Terminal_WriteString((uint8_t *)"No profile selected\n\r");
		return OverControlStatus_InvalidPointer;
	}

	// Check if we are ideling and have a valid profile pointer
	if(OverControlState_Idel == OvenParameters.state) {

		if(OvenParameters.profile->MaxStartTemperature <= OvenParameters.temperature) {
			
			OvenParameters.profileStep = 0;
			OvenParameters.profileDuration = 0;
			OvenParameters.timeLapse = 0;
			OvenParameters.state = OverControlState_Running;

			Terminal_WriteString((uint8_t *)"Oven started with profile: ");
			Terminal_WriteString(OvenParameters.profile->name);
			Terminal_WriteString((uint8_t *)"\n\r");
			return OverControlStatus_Ok;
		} else {
			Terminal_WriteString((uint8_t *)"Oven too hot to start\n\r");
			return OverControlStatus_TooHot;
		}

	}
	Terminal_WriteString((uint8_t *)"Oven already running\n\r");
	return OverControlStatus_InvalidPointer;
}


/**
 * manually set the temperature
 * 
 * @param temperature
 */
OverControlStatusEnum OvenControl_SetTemperature(float temperature) {
	OvenParameters.targetTemperature = temperature;
	return OverControlStatus_Ok;
}
/**
 *  Sets the motor power percentage
 * 
 * @param value with a range of 0 - 1
 */
void OverControl_SetFanPower(float value) {
	if (value > 1)  {
		value = 1;
	}
	OvenParameters.motorPwm = value;
	PWM_SetDuty(MotorPwmPin, value);
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

	OvenParameters.state = OverControlState_Idel;
}

/**
 * handles printing status messages to the screen
 */
void OvenControl_PrintStatus(void) {
	uint8_t MessageBuffer[100];
	snprintf((char *)&MessageBuffer[0], 100, 
			"T%4.2f,A%4.2f,H%4.2f,M%4.2f,C%d,L%d,D%d\n\r", 
			OvenParameters.temperature, 
			OvenParameters.targetTemperature,
			OvenParameters.heaterPwm,
			OvenParameters.motorPwm,
			OvenParameters.profileStep,
			OvenParameters.timeLapse,
			OvenParameters.profileDuration);
	Terminal_WriteString(&MessageBuffer[0]);
}
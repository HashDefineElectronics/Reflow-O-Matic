////////////////////////////////////////////////////////////////////////////////
/// \file OvenControl.h
/// Author: Ronald Sousa (@Opticalworm)
////////////////////////////////////////////////////////////////////////////////

#ifndef __OVEN_MONITOR_H__
#define __OVEN_MONITOR_H__
	#include "common.h"


	typedef enum {
		OverControlStatus_Ok = 0,
		OverControlStatus_Error,
		OverControlStatus_InvalidPointer,
		OverControlStatus_TooHot, ///< the chamber is too hot to start
	} OverControlStatusEnum;

	void OvenControl_Init(SolderProfileInterface * profile);
	void OvenControl_Process(float *currentTemp, float *currentTagetTemp, float *heaterPWM);
	OverControlStatusEnum OvenControl_SetTemperature(float temperature);
	OverControlStatusEnum OverControl_Run(void);
	void OvenControl_Stop(void);

#endif

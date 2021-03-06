////////////////////////////////////////////////////////////////////////////////
/// \file OvenControl.h
/// Author: Ronald Sousa (@Opticalworm)
////////////////////////////////////////////////////////////////////////////////

#ifndef __OVEN_MONITOR_H__
#define __OVEN_MONITOR_H__
	#include "common.h"


	void OvenControl_Init(void);
	void OvenControl_Process(float *currentTemp, float *currentTagetTemp, float *heaterPWM, uint32_t disableAll);
	void OvenControl_SetTemperature(float temperature);

#endif

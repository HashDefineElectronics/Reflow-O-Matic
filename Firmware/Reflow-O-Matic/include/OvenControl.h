////////////////////////////////////////////////////////////////////////////////
/// \file OvenControl.h
/// Author: Ronald Sousa (@Opticalworm)
////////////////////////////////////////////////////////////////////////////////

#ifndef __OVEN_MONITOR_H__
#define __OVEN_MONITOR_H__
    #include "SolderProfile/Profile.h"
	#include "common.h"


	typedef enum {
		OverControlStatus_Ok = 0,
		OverControlStatus_Error,
		OverControlStatus_InvalidPointer,
		OverControlStatus_TooHot, ///< the chamber is too hot to start
	} OverControlStatusEnum;

	void OvenControl_Init(SolderProfileInterface * profile);
	void OvenControl_Process(uint8_t *character);
	OverControlStatusEnum OvenControl_SetTemperature(float temperature);
	OverControlStatusEnum OverControl_Start(void);
	void OvenControl_Stop(void);
	void OverControl_SetFanPower(float value);
	void OvenControl_PrintStatus(void);

#endif

/**
 * \file Profile_CHIPQUIK_SMDLTLFP
 * 
 * This is the solder profile for ChipQuik SMDLTLFP lower temperature solder
 */
#include "SolderProfile/Profile.h"

static uint32_t ProfileTemperaure[] = {
  90,   ///< 90 seconds
  110,  ///< 180 seconds
  130,  ///< 180 seconds
  138,  ///< 210 Seconds
  165,  ///< 240 Seconds
  138,  ///< 270 Seconds
};

static uint32_t ProfileDuration[] = {
  90,   ///< 90 seconds
  45,   ///< 180 seconds
  45,   ///< 180 seconds
  30,   ///< 210 Seconds
  30,   ///< 240 Seconds
  30,   ///< 270 Seconds
};


SolderProfileInterface Profile_CHIPQUIK_SMDLTLFP = {
    .name = (uint8_t *)"CHIPQUIK - SMDLTLFP",
    .MaxStartTemperature = 30,
    .temperature = ProfileTemperaure,
    .duration = ProfileDuration,
    .NumberOfDataPoints = 6
};
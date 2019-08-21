/**
 * \file Profile_CHIPQUIK_SMDLTLFP
 * 
 * This is the solder profile for ChipQuik SMDLTLFP lower temperature solder
 */
#include "SolderProfile/Profile.h"

#define ChipquikProfileNumberOfDataPoints 5

static TemperaturePointType ProfileDataPoints[] = {
    //{30, 60},  ///< The first 60 seconds allows the board to warm up
    {90, 90},  ///< 90 seconds
    {130, 90}, ///< 180 seconds
    {138, 30}, ///< 210 Seconds
    {165, 30}, ///< 240 Seconds
    {138, 30}  ///< 270 Seconds
};



SolderProfileInterface Profile_CHIPQUIK_SMDLTLFP = {
    .points = {ProfileDataPoints},
    .NumberOfDataPoints = ChipquikProfileNumberOfDataPoints
};
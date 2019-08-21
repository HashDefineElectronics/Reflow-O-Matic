#ifndef __SOLDER_PROFILE_H__
#define __SOLDER_PROFILE_H__
    #include "../common.h"

    /**
     * This is the temperature data points
     */
    typedef struct {
        uint16_t temperature; ///< this the the desire temperature
        uint16_t seconds;     ///< this is how many seconds this temperature is going to be held for
    } TemperaturePointType;

    /**
     * defines the solder data profile
     */
    typedef struct {
        uint32_t NumberOfDataPoints;            ///< this is the number of data points in the points array
        uint16_t MaxStartTemperature;           ///< this is the maximun temperature that this profile can start
        TemperaturePointType * points[];
    } SolderProfileInterface;


    extern SolderProfileInterface Profile_CHIPQUIK_SMDLTLFP;
#endif /* __SOLDER_PROFILE_H__ */
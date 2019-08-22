#ifndef __SOLDER_PROFILE_H__
#define __SOLDER_PROFILE_H__
    #include "../common.h"

    /**
     * This is the temperature data points
     */
    typedef struct {
        uint32_t temperature; ///< this the the desire temperature
        uint32_t seconds;     ///< this is how many seconds this temperature is going to be held for
    } TemperaturePointType;

    /**
     * defines the solder data profile
     */
    typedef struct {
        uint32_t    NumberOfDataPoints;            ///< this is the number of data points in the points array
        uint16_t    MaxStartTemperature;           ///< this is the maximun temperature that this profile can start
        uint8_t     *name;                          ///< this is the string name for this profile
        uint32_t    *temperature;
        uint32_t    *duration;
    } SolderProfileInterface;



    extern SolderProfileInterface Profile_CHIPQUIK_SMDLTLFP;
#endif /* __SOLDER_PROFILE_H__ */
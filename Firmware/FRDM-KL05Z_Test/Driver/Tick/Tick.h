///////////////////////////////////////////////////////////////////////////////
/// \file Tick.h
/// \author Ronald Sousa
/// \website www.HashDefineElectronics.com
/// \company Hash Define Electronics Ltd
///
/// \brief  Hanldes generation a stable 1ms tick counter to create system delay
///
///
/// \setion Project Dependency
/// Compiler: Microchip XC32 v1.34
/// library: Microchip Harmony v1.03.01
/// IDE: Microchip MPLABX v2.26
///////////////////////////////////////////////////////////////////////////////

#ifndef __TICK_H__
#define __TICK_H__
    #include "../../common.h"

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief defines a non-blocking delay data type used by Tick_DelayMs_NonBlocking
    ///
    ///////////////////////////////////////////////////////////////////////////////
    typedef struct
    {
        uint32_t StartMs;       ///< this is the start tick time. internally used by Tick_DelayMs_NonBlocking
        uint32_t DelayMs;       ///< This is the desire delay time
    }TickType;

    void Tick_Init(void);
    uint32_t Tick_GetMs(void);
    int_fast8_t Tick_DelayMs_NonBlocking(uint_fast8_t reset, TickType * config);
    void Tick_DelayMs(uint32_t delayMs);
#endif

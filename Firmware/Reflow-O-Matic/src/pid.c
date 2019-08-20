///////////////////////////////////////////////////////////////////////////
/// \file heater.cpp
/// \brief interface layer for controlling the heaters
/// \author Ronald Sousa
///////////////////////////////////////////////////////////////////////////
#include "pid.h"

///////////////////////////////////////////////////////////////////////////
/// \brief Defines the sampling constant used by the PID: ie. dt 
/// (See Heater interrupt)
///////////////////////////////////////////////////////////////////////////
const float SamplingRateMs = 0.01;
///////////////////////////////////////////////////////////////////////////
/// \brief Defines the maximum the PID can set the driving PWM
///////////////////////////////////////////////////////////////////////////
const float MaximunOuput = 10.0;
const float LimitOuput = 5;
///////////////////////////////////////////////////////////////////////////
/// \brief Defines the minimum the PID can set the driving PWM
///////////////////////////////////////////////////////////////////////////
const float MinimumOuput = 0;

///////////////////////////////////////////////////////////////////////////
/// \brief Setup up the PID controller
///////////////////////////////////////////////////////////////////////////
void PID_Init(void)
{

}
///////////////////////////////////////////////////////////////////////////
/// \brief PID process controller. re-entry
/// \param pidParameter the pid parameter where the function stores is last
/// know error and also hold the PID characteristics
/// \param targetValue the target value to which the PID will aim for
/// \param actualValue the reading from the temperature sensor
///
/// \return return heater PWM value
///////////////////////////////////////////////////////////////////////////
float PID_Process(PIDType *pidParameter, const float targetValue, const float actualValue)
{
    float Result = 0;
    
    // Calculate the current error between our target value and 
    // what the sensor are reading
    float CurrentError = ((float)targetValue - actualValue);
    // Take the difference between last error and current error
    float DerivedError = ((CurrentError - pidParameter->LastError)/SamplingRateMs);
    // Take the sum of the last error
    pidParameter->IntergralError += (CurrentError*SamplingRateMs);
    
    // Calculate the new adjust PWM
    Result =    (pidParameter->P * CurrentError) +
                (pidParameter->I * pidParameter->IntergralError) +
                (pidParameter->D * DerivedError);

    // Now update the last error
    pidParameter->LastError = CurrentError;
    
    // Lets make sure that the Result are within limits
    if(Result > LimitOuput)
    {
        Result = LimitOuput;
       //return  1;//MaximunOuput;
    }
    else if(Result <= MinimumOuput)
    {
        Result = MinimumOuput;
        
    }
    
    //Return the new PWM value
    return (Result/MaximunOuput);
}

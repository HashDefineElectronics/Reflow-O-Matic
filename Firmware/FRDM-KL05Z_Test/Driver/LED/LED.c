/////////////////////////////////////////////////////////////////////////
///	\file led.c
///	\brief this is the LED hardware interface layer.
///
///	Author: Ronald Sousa (@Opticalworm)
/////////////////////////////////////////////////////////////////////////
	#include "LED.h"

/////////////////////////////////////////////////////////////////////////
///	\brief defines the LED pin number
/////////////////////////////////////////////////////////////////////////
#define LED_PIN	5

/////////////////////////////////////////////////////////////////////////
///	\brief	Turns on the LED
/////////////////////////////////////////////////////////////////////////
void LED_On(LED_Selector led)
{
	GPIOA->BSRR |= ((uint32_t)1 << LED_PIN);
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Turns off the LED
/////////////////////////////////////////////////////////////////////////
void LED_Off(LED_Selector led)
{
	GPIOA->BRR |= ((uint32_t)1 << LED_PIN);
}

/////////////////////////////////////////////////////////////////////////
/// \brief  Toggle the LED state
/////////////////////////////////////////////////////////////////////////
void LED_Toggle(LED_Selector led)
{
    // read the IO state. if set the turn the led off else turn it on
    if(GPIOA->ODR & (1<<LED_PIN))
    {
        IO_Off(led);
    }
    else
    {
        IO_On(led);
    }
}
/////////////////////////////////////////////////////////////////////////
///	\brief	Setup the LED IO
/////////////////////////////////////////////////////////////////////////
void LED_Init(void)
{

	IO_Off();
}
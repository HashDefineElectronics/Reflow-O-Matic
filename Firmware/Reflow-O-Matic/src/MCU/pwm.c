///////////////////////////////////////////////////////////////////////////
/// \file TempMonitor.cpp
/// \brief Hanldes controller the temperature of the oven. this will also control the cooling fan
/// \author Ronald Sousa
///////////////////////////////////////////////////////////////////////////
#include "MCU/pwm.h"

#define TIMER3_RESOLUTION 65536

///////////////////////////////////////////////////////////////////////////
/// \brief Configures the timer 3 for PWM
///
///	\param frequency the PWM interval
///	\param initialDuty the duty cycle all channels will start on
///////////////////////////////////////////////////////////////////////////
void PWM_Init(uint32_t frequencyHz, float initialtDuty)
{
	float DesireCount = SystemCoreClock / frequencyHz;
	float Prescale = ((DesireCount / (TIMER3_RESOLUTION -1)) + 0.9); // Prescale is the desire counter minors timer 3 resolution
	DesireCount += 0.9; // round up
	DesireCount = (uint32_t)DesireCount;
	Prescale = (uint32_t)Prescale;
	float AutoReloadValue =  TIMER3_RESOLUTION - ( (DesireCount) / Prescale); // by adding 0.5 and converting the result to float should round up our value


	RCC->AHBENR |= ((uint32_t)RCC_AHBENR_GPIOBEN); // enable port B clock
	RCC->APB1ENR |= ((uint32_t)RCC_APB1ENR_TIM3EN); // enable TIMER 3clock

	// set PB4 to alternate function
	GPIOB->MODER  &= ~((uint32_t)0x3 << 8);    	// clear the related bits before we set them.
	GPIOB->MODER |= ((uint32_t)2 << 8); 		//  set PB4 to alternate function
	GPIOB->AFR[0] |= ((uint32_t)1<<16);			// select AF1 = TIM3_CH1

	// set PB5 to alternate function
	GPIOB->MODER  &= ~((uint32_t)0x3 << 10);    // clear the related bits before we set them.
	GPIOB->MODER |= ((uint32_t)2 << 10); 		// set PB5 to alternate function
	GPIOB->AFR[0] |= ((uint32_t)1<<20);			// select AF1 = TIM3_CH2


	// configure timer3 controls the output compare 1 and 2
	TIM3->CCMR1 |= ((uint32_t) 6 << 4) | ((uint32_t) 6 << 12); 	// set OC1M and OC2M to PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; 			// Output Compare 1 and 2 Preload enable
	TIM3->CCMR1 &= ~((uint32_t)3 << 8); // clear  CC2S. this set CC2 as output
	TIM3->CCMR1 &= ~((uint32_t)3); // clear  CC1S. this set CC1 as output
	TIM3->CR1 |= TIM_CR1_ARPE;									// auto preload enabled
	TIM3->CCER |= ((uint32_t)(TIM_CCER_CC1E | TIM_CCER_CC2E)); // enable CC1 and CC2

	TIM3->PSC = Prescale; // prescale
	TIM3->ARR = TIMER3_RESOLUTION - AutoReloadValue; // auto reload register

	TIM3->CR1 |= TIM_CR1_CEN;
	TIM3->EGR |= TIM_EGR_UG;

	PWM_SetDuty(HeaterPwmPin, initialtDuty);
	PWM_SetDuty(MoterPwmPin, initialtDuty);






}

///////////////////////////////////////////////////////////////////////////
/// \brief set the select pwm pin to the given duty
///
///	\param pin the select pwm pin to set the duty ratio
///	\param duty the  duty value. from 0 - 1
///////////////////////////////////////////////////////////////////////////
void PWM_SetDuty(PWMPinSelectType pin, float duty)
{

	// clamp the duty ration to the range 0 - 1
	if ( duty < 0 ) {
		duty = 0;
	}
	else if ( duty > 1 ) {
		duty = 1;
	}

	switch(pin) {
		case MoterPwmPin:
			/*if (1 == duty)
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 4)) | ((uint32_t) 5<< 4); 	// force the output high
			}
			else if( 0 == duty )
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 4)) | ((uint32_t) 4<< 4); 	// force the output low
			}
			else
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 4)) | ((uint32_t) 6<< 4); 	// force the output PWM mode 1
				TIM3->CCR1 = TIM3->ARR * duty; // set the duty ration
			}*/

			TIM3->CCR1 = TIM3->ARR * duty; // set the duty ration
			break;

		case HeaterPwmPin:
			/*if (1 == duty)
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 12)) | ((uint32_t) 5<< 12); 	// force the output high
			}
			else if( 0 == duty )
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 12)) | ((uint32_t) 4<< 12); 	// force the output low
			}
			else
			{
				TIM3->CCMR1 =  (TIM3->CCMR1 & ((uint32_t) 7 << 12)) | ((uint32_t) 6<< 12); 	// force the output PWM mode 1
				TIM3->CCR2 = TIM3->ARR * duty; // set the duty ration
			}*/
			TIM3->CCR2 = TIM3->ARR * duty; // set the duty ration
			break;
	}

	//TIM3->EGR |= TIM_EGR_UG;
}




/*******************************************************************************
 * @file    Timer3_OnePulse.h
  * @author  Daniel Berger
  * @date    16-May-2017
  * @brief   This code demonstrate the use of the one pulse mode on a Timer (Timer3).
  *
  * 		 The pulse is sent to a  pin, using the pin's alternate function. For this
  *          example, the Pin 0 of port B (PB0) is used.
  *
  *			 The pulse is a PWM2 type, and we calculate the lenght of the pulse as:
  *
  *			     One Pulse value = [(TIM_Period - TIM_Pulse) / TIM_Period]  * Timer Period
  *
  *			     TIM counter clock
  *              For this example, the timer 3 has a frequency of 36000000, and a period
  *              setting of 65535:
  *
  *                    (65535 / 36000000) mSec or 1.82 mSec
  *
  *             And the PWD pulse is  48000, so
  *             [(65535- 48000)/65535] * 1.82 mSec = 0.487 mSec
  *
  *             --> But the reading on the Logic Analyser is the half of this, so 0.2435
  *
  *      Multiple Pulse:
  *            The timer can be setup to repeat the pulse (multiple pulse), instead of having a
  *            single pulse. The pulses we have the same form, as in the one pulse example
  *            For this, the bit OPM of the TIMx_CR1 has to be set to 0.
  *
  *	   General Steps (See methods below):
  *	     GPIO Pins:     Activate Clock,
  *	     				Set the PIN output, speed, etc
  *	     Timer:         Activate Clock,
  *	     				Set the general config (Period, Prescaler)
  *	     				Set the Output Compare value, for each channel to be used (4 here)
  *

 ********************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


GPIO_InitTypeDef GPIO_InitStructure;
__IO uint32_t TimingDelay;


void initGPIO()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//enable port B and alternate function
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO,ENABLE);

    //Init the LED bin of the board
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

	//init port B pin 0 = TIM3 channel 3 to alternate function push-pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void initTimer3OnePulse()
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		//set clock to TIM3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		//timer basestructure 24mhz/(0+1)/(0+1) ~ 2,72mS (655353/24000000)s for full period
		TIM_TimeBaseStructure.TIM_Prescaler = 0 ;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseStructure.TIM_Period = 65535 ;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


		/* TIM3 PWM2 Mode configuration: Channel1 */
		//for one pulse mode set PWM2, output enable, pulse (1/(t_wanted=TIM_period-TIM_Pulse)), set polarity high
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 48000;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC3Init(TIM3, &TIM_OCInitStructure);

		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

		/* OPM Bit -> Only one pulse */
		//*** Comment out the following line for multiple pulse!!!
		TIM3->CR1 |= (1 << 3);
		/* TIM3 enable counter */
	//	TIM_Cmd(TIM3, ENABLE);

}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);

  TimingDelay = 0;
}

void SysTick_Handler(void)
{
//	  TimingDelay_Decrement();
	  if (TimingDelay != 0x00)
	     {
	       TimingDelay--;
	     }

}

void main(void)
{
	//Use the System Tick for the flashing of the LED and activation of
	//Timer 3
	if (SysTick_Config(SystemCoreClock / 1000))
	{
	  /* Capture error */
	  while (1);
	}

	initGPIO();
	initTimer3OnePulse();

	//Flash the board's Led to indicate when a pulse is sent to the
	//PB0 pin... This part is for demo only.
	 while(1){
		 Delay(250);
		 TIM_Cmd(TIM3, ENABLE);
		 GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		 Delay(250);
		 TIM_Cmd(TIM3, DISABLE);
		 GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	 }



}

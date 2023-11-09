/******************************************************************************
  * @file    Timer3_GPIO_AlternateFunction.h
  * @author  Daniel Berger
  * @version V1.0.1
  * @date    01-April-2017
  * @brief   This code demonstrate the use of a Timer (Timer3) to change the state of
  *          the LED3,4,5 and 6 using a direct write on the pin's state using an alternate function.
  *
  *			 The four different channels of the timer (from 1 to 4) been used to show
  *			 how the compare between the current value of the timer and the respective
  *			 Output Compare of each channes (OCC1 to OCC4) can be done in parralel.
  *			 Once the value of the timer matches one of the Output Compare of a channel,
  *          the state of the pin is changed according to the settings done in the init
  *			 Like the previous example of TIM3, an event can also be triggered (a bit on the timer3
  *          but since the pin's state is driven by the alternate function circuit, the event
  *          is not adding anything to the execution of this program.
  *
  *	   General Steps (See methods below):
  *	     GPIO Pins:     Activate Clock,
  *	     				Set the PIN output, speed, etc
  *	     Timer:         Activate Clock,
  *	     				Set the general config (Period, Prescaler)
  *	     				Set the Output Compare value, for each channel to be used (4 here)
  *	     NVIC (Event):  Have to setup the Handler for timer3 TIM3_IRQHandler
  *
  *   The main() of the program has to:
  *
  *   				initGPIO()
  *  				initTimer3()
  *
  *******************************************************************************/

#include "Timer3_GPIO_AlternateFunction.h"




uint16_t capture = 0;

//Each of those value will be used to compare the current value of the counter, by their respective channel (1 to 4)

__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;





void initGPIO(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);



	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  //????  Should set the GPIO_AF_2 as


	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_2);

}

void initTimer3()
{

	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef  TIM_OCInitStructure;

	  uint16_t PrescalerValue = 0;

	  /** Enable the TIM3 global Interrupt
	  * This is optional, since the alternate function on the pin PE3 will handle
	  * the change of value of the pin, so the interupt is not required to do the work
	  *
	  * But if you want to monitor, or be notified when the Timer channel's (Here, channel #2
	  * of Timer 3 for this example) change the value of the pin, then un-comment the NVIC section
	  * below
	  */
//	 NVIC_InitTypeDef NVIC_InitStructure;
//	 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	 NVIC_Init(&NVIC_InitStructure);



	  /* TIM3 clock enable */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	  /* Compute the prescaler value */
	  PrescalerValue = (uint16_t) ((SystemCoreClock) / 72000000) - 1;

	  /* Time base configuration */
	  /* the Timer is a simple Up Counting timer, with a period of 65535, and compared
	   *  with the Capture Value of CCR2_Val (27309), which represent around half the period
	   *  size;
	   */
	  TIM_TimeBaseStructure.TIM_Period = 65535;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	  /* Prescaler configuration */
	  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);



	  /******************  CHANNEL #2  *************************/
	  /**
	   *   the other channels on the Timer 3 - Port E appears to be burned
	   *   Only PE2, which is channel #2 of timer 3 is still running...
	   *   Should be tested on new STM32F3 board to confirm this..
	   */
	  /* Output Compare Timing Mode configuration: Channel2 */
	  //Use the toggle mode to switch the value on the PE3 pin, once the timer
	  //value (TIM_CNT) matches the Output Compare value (TIMx_CCR2) of channel #2
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	  //It is not clear if the Preload has to be used here...
	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);
	  //TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

//	  /* TIM Interrupts enable */
	  // Un-Comment the next line only if you want to use the Timer's Interrupt Handler
//	  TIM_ITConfig(TIM3, TIM_IT_CC2 , ENABLE);

	  /* TIM3 enable counter */
	  TIM_Cmd(TIM3, ENABLE);

}


/**
 *  Like explained in the init function, the event on the Timer is optional, only if you want
 *  to be notified of the change of value on the pin...
 *  Here, we will change the value of a LED, to show if the Timer Handler is called...
 */
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    STM_EVAL_LEDToggle(LED6);
  }
}




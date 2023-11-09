/******************************************************************************
  * @file    Timer3_GPIO_Interupt.h
  * @author  Daniel Berger
  * @version V1.0.1
  * @date    01-April-2017
  * @brief   This code demonstrate the use of a Timer (Timer3) to change the state of
  *          the LED3,4,5 and 6 using the timer interrupt
  *
  *			 The four different channels of the timer (from 1 to 4) been used to show
  *			 how the compare between the current value of the timer and the respective
  *			 Output Compare of each channes (OCC1 to OCC4) can be done in parralel.
  *			 Once the value of the timer matches one of the Output Compare of a channel,
  *			 an event is triggered (a bit on the timer3 Status register is set),
  *			 and the Timer handler routine (TIM3_IRQHandler) is called
  *
  *			 A more sophisticated version of this could that the Timer's hardware
  *			 output the value directly on the PIN (using an alternate function of the pin)
  *			 Check for other version in the NoteBook folder...
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

#include "Timer3_GPIO_Interupt.h"




uint16_t capture = 0;

//Each of those value will be used to compare the current value of the counter, by their respective channel (1 to 4)

__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;


/**
 *   Utility method to turn on/off the LEDS, that is to be used by the interupt handler of timer 3
 *   This will make visible that for each type of Output Compare Event that is happening,
 *   only the corresponding LED (CC1 --> LED3 ... CC4 --> LED6) will be ON.
 */
void activateLED(int index)
{
	if (index == LED3)
	{
		STM_EVAL_LEDOn(LED3);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDOff(LED5);
		STM_EVAL_LEDOff(LED6);
	}
	else if (index == LED4)
	{
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOn(LED4);
		STM_EVAL_LEDOff(LED5);
		STM_EVAL_LEDOff(LED6);
	}
	else if (index == LED5)
	{
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDOn(LED5);
		STM_EVAL_LEDOff(LED6);
	}
	else if (index == LED6)
	{
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
		STM_EVAL_LEDOff(LED5);
		STM_EVAL_LEDOn(LED6);
	}

}


void initGPIO()
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_15;

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;


//	  if (Led != LED5)
//	     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	  else
//	     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void initTimer3()
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  TIM_OCInitTypeDef  TIM_OCInitStructure;

	  uint16_t PrescalerValue = 0;

	  /* TIM3 clock enable */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	  /* Enable the TIM3 gloabal Interrupt */
	  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  /* Compute the prescaler value */
	  PrescalerValue = (uint16_t) ((SystemCoreClock) / 72000000) - 1;

	  /* Time base configuration */
	  TIM_TimeBaseStructure.TIM_Period = 65535;
	  TIM_TimeBaseStructure.TIM_Prescaler = 0;
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	  /* Prescaler configuration */
	  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

	  /* Output Compare Timing Mode configuration: Channel1 */
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

	  /* Output Compare Timing Mode configuration: Channel2 */

	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

	  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

	  /* Output Compare Timing Mode configuration: Channel3 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

	  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

	  /* Output Compare Timing Mode configuration: Channel4 */
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

	  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);

	  /* TIM Interrupts enable */
	  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);

	  /* TIM3 enable counter */
	  TIM_Cmd(TIM3, ENABLE);

}

/******************************************************************************/
/*                 STM32F30x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f30x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    activateLED(LED3);
  }
  else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    activateLED(LED4);
  }
  else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    activateLED(LED5);
  }
  else
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    activateLED(LED6);
  }
}



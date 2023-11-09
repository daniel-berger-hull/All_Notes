/******************************************************************************************
  * @title        ADC_Tim3_Trigger.c
  * @Description  Show how to use a timer (Timer #3) to trigger the reading of the ADC,
  * 	          instead of polling in a loop (like in the ADC_Polling.c example).
  *
  *               The timer and channel working toghetter are very specific: Is is not
  *               all timers (i.e Tim2, Tim3, etc) than can trigger any channel on the
  *               ADC. Refer to the F103 Datasheet, section 3 (GPIO). But for this
  *               example, the Timer #3 can trigger the ADC channel #6
  *
  * 		      The reading, done in the interrupt handler of the ADC (ADC1_2_IRQHandler)
  * 		      is done the same way than the polling example (ADC_Polling.c), which is
  * 		      to invoke the method
  * 					      			ADC_GetConversionValue
  * 		      It is safe however to declare the reading variable (reading) 'volatile',
  * 		      so that all the updates on the reading variable will be visible by all part
  * 		      of code that will have to access/modify this variable.
  *
  * 		      on a regular basis.
  *
  * 		      To have a range of analog value, use a potentiometer, and connected
  * 		      the STM32 pin to the center pin of the potentiometer (this is where the
  * 		      value of the voltage will varie with the position of the potentiometer)
  *
  *
  * 	Connection with the STM32F103C8T6:
  *                  STM32			potentiometer
  *				    -------------------------------
  *   				  A6 	 -->    Center Pin
  *
  *******************************************************************************************/
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#define   TRUE      1
#define   FALSE     0

volatile int reading;
volatile int readingAvailable = FALSE;

void GPIO_Configuration(void);
void ADC_Configuration(void);


/*******************************************************************************
 * Function Name  : GPIO_Configuration
 * Description    : Configures the pin for the analog read on PA6.
 *                  Could be any pin on the Port A, but Pin 6 as an alternate
 *                  function that output the Timer #3, which can be used
 *                  as trigger for the conversion
 *
 *                  There are a total of 16 channels (different pins) that can
 *                  be used as analog input pin to be read by the ADC 1 & 2:
 *
 *                  Port A  --> PA0 (Channel 0) to PA7 (Channel 7)
 *                  Port B  --> PB0 (Channel 8) to PB1 (Channel 9)
 *                  Port C  --> PC0 (Channel 10) to PC5 (Channel 15)
 *                  All those pin/channel can be set with the function
 *
 *                  ADC_RegularChannelConfig(ADC Num, Channel Num, Rank , State)
 *
  *******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;		// that's ADC1 (PA6 on STM32)
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void ADC_Configuration()
{
	ADC_InitTypeDef ADC_InitStructure;

	//clock for ADC (max 14MHz --> 72/6=12MHz)
	RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	// enable ADC system clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// define ADC config
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;;	// we work in continuous sampling mode
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1,ADC_Channel_6, 1,ADC_SampleTime_28Cycles5); // define regular conversion config
	ADC_Init ( ADC1, &ADC_InitStructure);	//set config of ADC1

	// enable ADC
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1

	//	ADC calibration (optional, but recommended at power on)
	ADC_ResetCalibration(ADC1);	// Reset previous calibration
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	// Start new calibration (ADC must be off at that time)
	while(ADC_GetCalibrationStatus(ADC1));

	// start conversion
	ADC_Cmd (ADC1,ENABLE);	//enable ADC1
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	// start conversion (will be endless as we are in continuous mode)

	//This line will make the execution of the ADC Interrupt Handler ADC1_2_IRQHandler possible.
	//If you omit this line, then no ADC interrupt will be processed
	//You will notice that the interupt flag is the 'E'nd 'O'f 'C'onversion, (ADC_IT_EOC)
	//which arise when the ADC is done with threading
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
}


/* As discussed before, the Timer #3 will trigger the moment where are reading will
 * be done by the ADC. You will see below that the update event of the timer is
 * redirected to trigger the ADC reading, by the method
 *                     TIM_SelectOutputTrigger(TimerNumer, TypeOfEvent);
 *                i.e: TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
 *
 */
void Timer3_Configuration()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	 //Timer #3 is on the Half Bus, running at 36 MHz. Dividing by 36000 give 1000Hz
	//which is not too fast for this example...
	TIM_TimeBaseStructure.TIM_Prescaler = 36000;
	//A period of 1000 tick will give an analog reading every second
	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Select the TIM3 update event (when it reaches it's max)
	 * to trigger the ADC reading 								*/
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}


/**
 * @brief  Configures the nested vectored interrupt controller.
 * @param  None
 * @retval None
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(ADC1_2_IRQn);
}



void  ADC1_2_IRQHandler(void)
{
	// read ADC DR and keep the value for the main loop
	ADC_ClearITPendingBit(ADC1 , ADC_IT_EOC);
	reading  = ADC_GetConversionValue(ADC1);
	readingAvailable = TRUE;
}


void delay_ms(__IO uint32_t nCount)
{
	uint32_t delay = nCount * 2516;              // approximate loops per ms at 24 MHz, Debug config
	for(; delay != 0; delay--);
}

int main(void)
{
	GPIO_Configuration();
	ADC_Configuration();
	Timer3_Configuration();
	NVIC_Configuration();

	while (1)
	{
		if (readingAvailable == TRUE)
		{
			//Something been read in the variable 'reading'
			//Do something with it...
		    readingAvailable = FALSE;
		}
	}
}






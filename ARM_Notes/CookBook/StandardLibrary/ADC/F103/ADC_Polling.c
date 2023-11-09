/******************************************************************************************
  * @title        ADC_Polling.c
  * @Description  Show how to read an analog value on a pin using the ADC peripheral ADC1
  * 		      In this example, the channel #6 of the ADC (which is connected to PA6)
  * 		      will be used.
  *
  * 		      The polling is the simplest way to read the ADC, because the main loop
  * 		      has to call the reading function
  * 		      			ADC_GetConversionValue
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

GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;



volatile int reading;

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
	//clock for ADC (max 14MHz --> 72/6=12MHz)
		RCC_ADCCLKConfig (RCC_PCLK2_Div6);
	// enable ADC system clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// define ADC config
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	// we work in continuous sampling mode
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
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

	while (1)
	{

		int reading  = ADC_GetConversionValue(ADC1);
		delay_ms(2000);
	}
}






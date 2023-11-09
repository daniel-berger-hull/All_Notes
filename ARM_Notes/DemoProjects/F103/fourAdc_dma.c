/**
  *****************************************************************************
  * @title   fourAdc_dma.c
  * @Description  Show how to use a dma to transfer the value four values read by the ADC
  *              peripheral. For this, the ADC has to be in  Continuous conversion (never stops),
  * 		     so that the DMA always has values to transfer.
  *
  *
  *
  * 	          The DMA then replace a polling in a loop (like in the ADC_Polling.c example),
  * 	          or a conversion triggered by a timer (ADC_Tim3_Trigger.c)
  *
  *				  It is relevant to note that in this demo code, there is no explicit
  *				  call to the ARC read (the ADC_GetConversionValue method). The DMA transfer
  *				  automatically into the reading variable.
  *
  * 		      To have a range of analog value, use a potentiometer, and connected
  * 		      the STM32 pin to the center pin of the potentiometer (this is where the
  * 		      value of the voltage will varie with the position of the potentiometer)
  *
  *                Finally, the reading is sent to the UART2 (speed 115200 Bauds) to be
  *                displayed into a Serial Console on a PC. Make sure that you connect a
  *                USB-TTL converted on Pins:
  *
  *                					A2 (TX)
  *                					A3 (RX)
  *
  *
  * 	Connection with the STM32F103C8T6:
  * 	(You may need 4 potentiometer for the test)
  *                  STM32			potentiometer
  *				    -------------------------------
  *   				  A4 	 -->    Center Pin
  *   				  A5 	 -->    Center Pin
  *   				  A6 	 -->    Center Pin
  *   				  A7 	 -->    Center Pin
  *
  *
  *******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"

#include "UART.h"


/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
__IO uint16_t ADCConvertedValue;

uint16_t ADCBuffer[] = {0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA};


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
void ADC_Configuration(void);


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4);
}

//We are going to use 4 pins and read all those pins, instead of only one!
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;		// Using 4 pins for Analog reading
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void DMA_Configuration(void)
{
	/* Enable DMA1 clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	  DMA_DeInit(DMA1_Channel1);
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBuffer;  //Drop the reading value into the value array
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = 4;                   //Has to transfer 4 values instead of one...
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	  /* Enable DMA1 channel1 */
	  DMA_Cmd(DMA1_Channel1, ENABLE);



}

void ADC_Configuration()
{
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	  /* ADC1 configuration ------------------------------------------------------*/
	  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	  ADC_InitStructure.ADC_NbrOfChannel = 4;   //Should that value be 4 instead???
	  ADC_Init(ADC1, &ADC_InitStructure);

	  /* ADC1 regular channel6 configuration */

	  ADC_RegularChannelConfig(ADC1,ADC_Channel_4, 1,ADC_SampleTime_55Cycles5);
	  ADC_RegularChannelConfig(ADC1,ADC_Channel_5, 2,ADC_SampleTime_55Cycles5);
	  ADC_RegularChannelConfig(ADC1,ADC_Channel_6, 3,ADC_SampleTime_55Cycles5);
	  ADC_RegularChannelConfig(ADC1,ADC_Channel_7, 4,ADC_SampleTime_55Cycles5);

	  /* Enable ADC1 DMA */
	  ADC_DMACmd(ADC1, ENABLE);

	  /* Enable ADC1 */
	  ADC_Cmd(ADC1, ENABLE);

	  /* Enable ADC1 reset calibration register */
	  ADC_ResetCalibration(ADC1);
	  /* Check the end of ADC1 reset calibration register */
	  while(ADC_GetResetCalibrationStatus(ADC1));

	  /* Start ADC1 calibration */
	  ADC_StartCalibration(ADC1);
	  /* Check the end of ADC1 calibration */
	  while(ADC_GetCalibrationStatus(ADC1));

	  /* Start ADC1 Software Conversion */
	  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void main(void)
{
  int i, AD_value;

  unsigned char outputMessage[50];
  memset(outputMessage,0,sizeof(outputMessage));

  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  USART_Configuration();

  GPIO_Configuration();
  DMA_Configuration();
  ADC_Configuration();

  printUSART("Initatialisation completed...\r\n");

  while (1)
  {
    for(i=0; i<1000000; i++);

//    sprintf(outputMessage,"Val = %d\n\r",ADCConvertedValue);
    sprintf(outputMessage,"%d %d %d %d\n\r",ADCBuffer[0],ADCBuffer[1],ADCBuffer[2],ADCBuffer[3]);



    printUSART(outputMessage);
  }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

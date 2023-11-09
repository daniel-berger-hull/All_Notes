/******************************************************************************
  *                                    BlackBoard_LED.c
  *
  *   This is a GPIO demo on the 2 User Leds that the STM32F4Black board has
  *
  *    Led:
  *       D2 (Top One)       -->  Pin PA6
  *       D3 (Bottom One)    -->  Pin PA7
  *
  *******************************************************************************/

#include "stm32f4_discovery.h"

__IO uint32_t TimingDelay = 0;


/**************************
 *  System Timer Handler
 *  In an Atollic TrueStudio project, this method can be moved to the handler file
 *  stm32f4xx_it.c  to keep consistency with TrueStudio coding organization.
 *
 */
void SysTick_Handler(void)
{
	 TimingDelay--;
}



void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}


void initGPIO()
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Start with LEDs turned off (so put high value on the pin)
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
}




int main(void)
{

	 /* Using the System SysTick Timer for 1 mSec interrupts  */
	  if (SysTick_Config(SystemCoreClock / 1000))
	  {
	    /* Capture error */
	    while (1)
	    {}
	  }

	  initGPIO();

  while (1)
  {
	  //Turn Led OFF by setting value High
	  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	  GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
	  Delay(200);

	  //Turn Led ON by setting value LOW
	  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	  GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
	  Delay(200);
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



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

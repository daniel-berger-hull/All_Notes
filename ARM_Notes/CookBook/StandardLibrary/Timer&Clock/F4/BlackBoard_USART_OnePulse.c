/******************************************************************************
  *                                    BlackBoard_USART_OnePulse.c
  *
  *   This is a USART demo that transmit to the USART peripheral configured (see
  *   USE_USARTx define in usart.c), and accept user command from the USART client
  *   running on the PC.
  *   It also send output to 2 pins (PB6 & PB7), which a driven by the Timer 4
  *   using the One Pulse Mode.
  *
  *   A flashing led method is written to have a physical display visible on the board
  *   (to show activity with want)
  *
  *******************************************************************************/

#include "stm32f4_discovery.h"
#include "stm32f4xx_tim.h"

#include "usart.h"

__IO uint32_t TimingDelay = 0;
uint16_t uhPrescalerValue = 0;


extern int commandToProcess;
extern uint8_t *userEntry;
extern uint8_t **keywords;

uint8_t sendData;

TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef       TIM_OCInitStructure;




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


void blinkLEDSequence()
{
	for (int i=0;i<3;i++)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
		 Delay(200);

		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
		Delay(200);
	}
}


void initGPIO()
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
							   RCC_AHB1Periph_GPIOB ,
							   ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Start with LEDs turned off (so put high value on the pin)
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);


	/////////////////////////////////////////////////
	/* TIM4_CH1 pin (PB.06) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* TIM4_CH2 pin (PB.07) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//One Pulse Alternate functions on the Timer 4
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);

}



void initTimer4()
{

	/* Compute the prescaler value */
	uhPrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 42000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_Prescaler = uhPrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* TIM4 PWM2 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 16383;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = 35000;

	TIM_OC2Init(TIM4, &TIM_OCInitStructure);

	/* One Pulse Mode selection */
	//TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Single);
	/* Uncomment the next one if you want multiple Pulse*/
	TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Repetitive);



//		TIM2->CCMR1 |= 0x0100; // Ch. 2 as TI2
//		 TIM2->SMCR |= 0x0007; // Ext. clk mode 1
//		 TIM2->SMCR |= 0x0060; // TI2FP2 as ext. clock
//		 TIM2->CR1 |= 0x0001; // enable counting


}

void displayMessage(char* message)
{
	char outputMessage[50];

	 strcpy(outputMessage,message);
	 USART_print(outputMessage);
}



int main(void)
{
	  char outputMessage[50];

	 /* Using the System SysTick Timer for 1 mSec interrupts  */
	  if (SysTick_Config(SystemCoreClock / 1000))
	  {
	    /* Capture error */
	    while (1)
	    {}
	  }

	  initGPIO();
	  USART_Configuration();
	  initTimer4();


	  	 strcpy(outputMessage,"Basic Initialisation Completed\n\r");
	  	 USART_print(outputMessage);

	  	blinkLEDSequence();

	  	 while (1)
	  	  {
	  	    if (commandToProcess == TRUE)
	  	    {
	  	    	int ret = parseUserEntry();
	  	        if (ret != UNKNOWN_ENTRY){


	  	        	if (ret == 1)
	  	        	{

	  	        		displayMessage("Running process\n\r");

	  	        		for (int i=0;i<10;i++)
	  	        		{
	  	        			TIM_Cmd(TIM4, ENABLE);
	  	        			GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	  	        			Delay(250);
	  	        			TIM_Cmd(TIM4, DISABLE);
	  	        			GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	  	        			Delay(250);
	  	        		}

	  	        		displayMessage("Process Completed!!!\n\r");


	  	        	}
	  	        	else if (ret == 2)
	  	        	{
	  	        		sendData = 1;
	  	        		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	  	        		displayMessage("Start Sending data...\n\r");
	  	        	}
	  	        	else if (ret == 3)
	  	        	{
	  	        		sendData = 0;
	  	        		GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	  	        		displayMessage("Data Transfer completed!\n\r");
	  	        	}
	  	        	else if (ret == 4)
	  	        	{
	  	        		displayMessage("Reset System...\n\r");
	  	        		blinkLEDSequence();
	  	        	}
	  	    	}
	  	        else
	  	        {
	  	        	displayMessage("Invalid command!\n\r");
	  	        }
	  	    	 resetUserEntry();
	  	    }

//	  	    if (sendData == 1)
//	  	    {
//	  	    	USART_directprint(batchBlock);
//	  	    }


	  	    Delay(250);
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



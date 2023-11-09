/******************************************************************************
*                        User Commands Processor Demo
*
*   This demo uses my libs usart.h and usart.c, so display UART message and send
*   command to an STM32F303 MCU.
*
*    The program react to following commands:
*     		start:    Send a block of 1K to the PC,and signal the transfer by
*     		          turning on a red led on the board
*     		stop:     Potential command to abort the previous 'start' command
*     		reset:    No function programmed, but recognized by the MCU
*     		transfer: No function programmed, but recognized by the MCU
*           run:      No function programmed, but recognized by the MCU
******************************************************************************/


#include "main.h"

__IO uint32_t TimingDelay = 0;


extern int commandToProcess;
extern uint8_t *userEntry;
extern uint8_t **keywords;

uint8_t sendData;


void initGPIO()
{

	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 |  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}



void blinkLEDSequence()
{
	for (int i=0;i<3;i++)
	{
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);   //Can set any pin from 8 to  15
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);   //Can set any pin from 8 to  15
		 Delay(200);

		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);  //Can reset any pin from 8 to  15
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_RESET);   //Can set any pin from 8 to  15
		 Delay(200);
	}
}

void displayMessage(char* message)
{
	char outputMessage[50];

	 strcpy(outputMessage,message);
	 USART_print(outputMessage);
}


int main()
{
	char batchBlock[1024];
	sendData = 0;

	memset(batchBlock,10,1024);

  /* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */
    while (1)
    {}
  }

  initGPIO();
  USART_Configuration();

  displayMessage("Basic Initialisation Completed\n\r");
  USART_directprint("This is a direct print to USART\n\r");


 //Visual Indication to the user that the MCU been restarted
 blinkLEDSequence();

  while (1)
  {
    if (commandToProcess == TRUE)
    {
    	int ret = parseUserEntry();
        if (ret != UNKNOWN_ENTRY){

        	if (ret == 2)
        	{
        		sendData = 1;
        		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);   //Can set any pin from 8 to  15
        		displayMessage("Start Sending data...\n\r");
        	}
        	else if (ret == 3)
        	{
        		sendData = 0;
        		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);   //Can set any pin from 8 to  15
        		displayMessage("Data Transfer completed!\n\r");
        	}
    	}
        else
        {
        	displayMessage("Invalid command!\n\r");
        }
    	 resetUserEntry();
    }

    if (sendData == 1)
    {
    	USART_directprint(batchBlock);
    }


    Delay(250);
  }
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
}




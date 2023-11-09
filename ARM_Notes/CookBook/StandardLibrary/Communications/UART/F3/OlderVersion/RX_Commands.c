/*****************************************************************************
 * @title   RX_Commands.c
 * @date    15 June 2016
 * @brief   This UART demo receive commands from the PC (via UART1) and send text
 *          to the PC.
 *
 *          To know the commands that the program will recognize, see the
 *          char array ** keywords **. The Red light will blink at every commands typed
 *          in the console on the PC, and also, a green light will blink for any key typed
 *          on the keyboard, confirming that the characted been received on the STM32 side.
 *
 *          The UART adapter used is an USB FTDI (little orange one). Due to the pins
 *          used by the LCD on the PORT A (where the UART1 pins reside), we used the
 *          UART2 instead,
 *
  *  Connection with the STM32F3:
  *                  STM32			 UART Adapter
  *				    ---------------------------------
  *                   PA9  			 --> TX
  *                   PA10			 --> RX
 ********************************************************************************/

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_usart.h"
#include "stm32f30x_misc.h"

#include "stm32_delay.h"

#include <string.h>
#include <stdio.h>

#include "usart.h"

#define TRUE 		 1
#define FALSE 		 0

#define NUM 		   20
#define RETURN_KEY	   13
#define UNKNOWN_ENTRY   0


extern uint8_t USART1_ringbuf[RINGBUF_SIZE];
extern uint32_t USART1_readidx;
extern uint32_t USART1_writeidx;

volatile uint8_t userEntry[NUM+1] = {'\0'};
uint8_t keywords[4][10] = {"run","start","stop","reset"};

volatile int receivedChar = FALSE;
volatile int commandToProcess = FALSE;
uint32_t nextCharIndex = 0;
volatile int bytesReceived = FALSE;

void InitializeLEDs()
{
	GPIO_InitTypeDef gpioStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

    gpioStructure.GPIO_Pin =  GPIO_Pin_9 |     //Red LED top - reacts to key enter from the user and confirm processing of command
    					      GPIO_Pin_11;     //Green LED right - A char been received on UART 1 (key been entered on the PC)

    gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
    gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;  //This will pull up the pin, so if we:
    										 // i)  Write a Bit_SET   (value 1) --> LED will Light ON
    										 // ii) Write a Bit_RESET (value 0) --> Turn of the LED

    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpioStructure);
}

void InitializeUSART()
{
	  /* enable usart clock */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	    GPIO_InitTypeDef GPIO_InitStructure;
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // GPIO_Pin_8 |
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);

	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
	    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);

		USART_DeInit(USART1);

	    USART_InitTypeDef USART_InitStructure;
	    USART_StructInit(&USART_InitStructure);
	    USART_InitStructure.USART_BaudRate = 115200;
	    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	    USART_InitStructure.USART_StopBits =  USART_StopBits_1;
	    USART_InitStructure.USART_Parity =    USART_Parity_No;
	    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	    USART_Init(USART1, &USART_InitStructure);

	    USART_Cmd(USART1, ENABLE);

		USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);

		bytesReceived = FALSE;
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_EnableIRQ(USART1_IRQn);
}

void resetUserEntry(void)
{
	memset(userEntry, 0, NUM+1);
	nextCharIndex = 0;
	bytesReceived = FALSE;
	commandToProcess = FALSE;
}

int parseUserEntry(uint8_t entry[])
{

	for (int j=0;j<4;j++)
	{
		if (strcmp(entry, keywords[j]) == 0)
			//The plus one is because the j loop and the array starts at index 0,
			//but if we send back index o (first command in the array of commands),
			//that is going to create a problem because unknow code is also 0
			return (j+1);
	}

	return UNKNOWN_ENTRY;
}

void main(void)
{
	uint8_t outputMessage[50];
	strcpy(outputMessage,"Waiting for bytes...\n\r");

	 InitializeLEDs();
	 InitializeUSART();
	 NVIC_Configuration();

	GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_RESET);   //Can set any pin from 8 to  15
	GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);   //Can set any pin from 8 to  15

//	USART1_print("From write interrupt!!\n\r");
	USART1_print(outputMessage);

	while(1){

		if (commandToProcess == TRUE)
		{
			int ret = parseUserEntry(userEntry);
		    if (ret != UNKNOWN_ENTRY){
		    	strcpy(outputMessage,"Execute ");
		    	strcat(outputMessage, keywords[ret-1]);
		    	strcat(outputMessage, "\n\r");

		    	USART1_print(outputMessage);
			}

			 GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);   //Can set any pin from 8 to  15
			 Delay_ms(200);
			 GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);   //Can set any pin from 8 to  15
			 resetUserEntry();
		}
		else if (bytesReceived == TRUE)
		{
			 GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_SET);   //Can set any pin from 8 to  15
			 Delay_ms(200);
			 GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_RESET);   //Can set any pin from 8 to  15
			bytesReceived = FALSE;
		}

	}
}

void USART1_IRQHandler(void)
{
    //the transmitting used in this demo is asychronus, meaning that the MCU does not wait for the next
	//byte to be transmitted or received, but delegate the work to this interrupt method to do the
	//required work only when the appropriate flag is set in the Status Flag Register
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{

		if (USART1_writeidx - USART1_readidx == 0) {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			return;
		}

		USART_SendData(USART1, USART1_ringbuf[(USART1_readidx++) & (RINGBUF_SIZE-1)]);
	}
	else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		receivedChar = USART_ReceiveData(USART1);

		if  (receivedChar == RETURN_KEY)
			commandToProcess = TRUE;
		else if(nextCharIndex == NUM)
		 	resetUserEntry();
		 else
		 {
			userEntry[nextCharIndex++] = receivedChar;
			userEntry[nextCharIndex] = '\0';
			bytesReceived = TRUE;
		 }
    }
}



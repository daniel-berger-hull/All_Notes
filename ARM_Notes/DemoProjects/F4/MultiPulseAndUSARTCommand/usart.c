/******************************************************************************************
 *									STM32F4 USART Library
 *
 *    Includes a user commands processing framework
 *
 *  This library use preprocessor variable (#define) to select which USART peripheral will be
 *  used. Make sure to modify a define statement below accordingly
 *
 *
 *   STM32F4 USART Pins:
 *   -------------------
 *   Many different pin options are available, but here are few common setups
 *
 *   USART1: PA9  --> TX ,   PA10  --> RX
 *   USART2: PA2  --> TX ,   PA3   --> RX
 *   USART3: PB10 --> TX ,   PB11  --> RX
 *
 *   Simple Usage:
 *   --------------
 *
 *      #include <stdio.h>
 *
 *		int main(void) {
 *			char outputMessage[50];
 *
 *      	strcpy(outputMessage,"This is an USART printout\n\r");
 *  		USART2_Init();
 *    		USART_print(outputMessage);
 *     }
 *
 * For a more complete example, see the *** usart_command.c *** for a functionnal
 * user commands processor
 *
 *******************************************************************************************/


#include <strings.h>

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"

#include "usart.h"


//  You must define a USART number (from 1 to 3), otherwise the USART peripheral isn't defined
#define USE_USART1


#define MIN(a, b) ((a)<(b)?(a):(b))


#define TOTAL_NUMBER_COMMANDS  5

volatile uint8_t  USART_ringbuf[RINGBUF_SIZE];
volatile uint32_t USART_readidx = 0;
volatile uint32_t USART_writeidx = 0;

volatile uint16_t receivedChar = 0;


volatile uint8_t userEntry[NUM+1] = {'\0'};
//This is a simple list of user commands that the library can respond to
uint8_t keywords[TOTAL_NUMBER_COMMANDS][10] = {"run","start","stop","reset","transfer"};

volatile int commandToProcess = FALSE;
uint32_t nextCharIndex = 0;
volatile int bytesReceived = FALSE;




void USART_Configuration(void)
{
	USART_GPIO_Configuration();

	/* enable usart clock */
	#ifdef  USE_USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		USART_DeInit(USART1);
	#elif defined  (USE_USART2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		USART_DeInit(USART2);
	#elif  defined  (USE_USART3)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		USART_DeInit(USART3);
	#endif


	USART_InitTypeDef USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits =  USART_StopBits_1;
	USART_InitStructure.USART_Parity =    USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	#ifdef  USE_USART1
		    USART_Init(USART1, &USART_InitStructure);
		    USART_Cmd(USART1, ENABLE);
			USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
	#elif  defined  (USE_USART2)
		    USART_Init(USART2, &USART_InitStructure);
		    USART_Cmd(USART2, ENABLE);
		    USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
	#elif  defined  (USE_USART3)
		    USART_Init(USART3, &USART_InitStructure);
		    USART_Cmd(USART3, ENABLE);
		    USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
	#endif


	USART_NVIC_Configuration();
}

void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;


	#ifdef  USE_USART1
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

//		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
//		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	#elif  defined  (USE_USART2)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	#elif  defined  (USE_USART3)
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	#endif

}


void USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* Enable the USARTx Interrupt */
	#ifdef  USE_USART1
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_EnableIRQ(USART1_IRQn);
	#elif  defined  (USE_USART2)
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_EnableIRQ(USART2_IRQn);
	#elif  defined  (USE_USART3)
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_EnableIRQ(USART3_IRQn);
	#endif

}



void USART_putc(char ch)
{
	while (1) {
		uint32_t capacity = RINGBUF_SIZE - (USART_writeidx - USART_readidx);
		if (capacity > 0) break;
	}
	USART_ringbuf[(USART_writeidx++) & (RINGBUF_SIZE-1)] = ch;
}

void USART_write(const char *str, int len)
{
	uint32_t i = 0;
	while (i < len) {
		uint32_t writeidx = USART_writeidx & (RINGBUF_SIZE-1);
		uint32_t len_to_end = RINGBUF_SIZE - writeidx;
		uint32_t capacity = RINGBUF_SIZE - (USART_writeidx - USART_readidx);
		uint32_t max_len = MIN(len_to_end, capacity);
		if (max_len == 0) continue;

		uint32_t this_len = MIN(max_len, len - i);

		int j;
		for (j = 0; j < this_len; ++j) {
			USART_ringbuf[writeidx++] = str[i++];
		}
		USART_writeidx += this_len;

		#ifdef  USE_USART1
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		#elif  defined  (USE_USART2)
			USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
		#elif  defined  (USE_USART3)
			USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
		#endif
	}
}

void USART_print(const char *str)
{
	uint32_t len = strlen(str);
	USART_write(str, len);
}

void USART_directputc(const char ch)
{
	#ifdef  USE_USART1
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, ch);
	#elif  defined  (USE_USART2)
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, ch);
	#elif  defined  (USE_USART3)
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, ch);
	#endif

//	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//	USART_SendData(USART2, ch);
}

void USART_directprint(const char *str)
{
	uint32_t i = 0;
	uint32_t len = strlen(str);
	while (i < len) {
		USART_directputc(str[i++]);
	}
}

void USART_flush(void)
{
	while (USART_readidx != USART_writeidx);
}


#ifdef  USE_USART1
void USART1_IRQHandler(void)
{
	//the transmitting used in this demo is asychronus, meaning that the MCU does not wait for the next
	//byte to be transmitted or received, but delegate the work to this interrupt method to do the
	//required work only when the appropriate flag is set in the Status Flag Register
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{

		if (USART_writeidx - USART_readidx == 0) {
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			return;
		}

		USART_SendData(USART1, USART_ringbuf[(USART_readidx++) & (RINGBUF_SIZE-1)]);
	}
	else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		receivedChar = USART_ReceiveData(USART1);

		if  (receivedChar == RETURN_KEY)
			commandToProcess = TRUE;
		else if(receivedChar == NUM)
			resetUserEntry();
		else
		{
			userEntry[nextCharIndex++] = receivedChar;
			userEntry[nextCharIndex] = '\0';
			bytesReceived = TRUE;
		}
	}
}
#endif

#ifdef  USE_USART2
void USART2_IRQHandler(void)
{
	//the transmitting used in this demo is asychronus, meaning that the MCU does not wait for the next
	//byte to be transmitted or received, but delegate the work to this interrupt method to do the
	//required work only when the appropriate flag is set in the Status Flag Register
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{

		if (USART_writeidx - USART_readidx == 0) {
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			return;
		}

		USART_SendData(USART2, USART_ringbuf[(USART_readidx++) & (RINGBUF_SIZE-1)]);
	}
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		receivedChar = USART_ReceiveData(USART2);

		if  (receivedChar == RETURN_KEY)
			commandToProcess = TRUE;
		else if(receivedChar == NUM)
			resetUserEntry();
		else
		{
			userEntry[nextCharIndex++] = receivedChar;
			userEntry[nextCharIndex] = '\0';
			bytesReceived = TRUE;
		}
	}
}

#endif

#ifdef  USE_USART3
void USART3_IRQHandler(void)
{
	//the transmitting used in this demo is asychronus, meaning that the MCU does not wait for the next
	//byte to be transmitted or received, but delegate the work to this interrupt method to do the
	//required work only when the appropriate flag is set in the Status Flag Register
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{

		if (USART_writeidx - USART_readidx == 0) {
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			return;
		}

		USART_SendData(USART3, USART_ringbuf[(USART_readidx++) & (RINGBUF_SIZE-1)]);
	}
	else if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		receivedChar = USART_ReceiveData(USART3);

		if  (receivedChar == RETURN_KEY)
			commandToProcess = TRUE;
		else if(receivedChar == NUM)
			resetUserEntry();
		else
		{
			userEntry[nextCharIndex++] = receivedChar;
			userEntry[nextCharIndex] = '\0';
			bytesReceived = TRUE;
		}
	}
}

#endif

/******************************************************************************************************
 *   						SECTION of COMMAND PROCESSING FUNCTIONS
 *
 *   resetUserEntry:  Clear all the variables to be ready to listen and accept the next command
 *                    coming on the USART port
 *   parseUserEntry:  Utility method to detect the string of a command, among those allowed by the
 *                    library.  Each commands must end by byte value of 0
 *
 *******************************************************************************************************/
void resetUserEntry(void)
{
	memset(userEntry, 0, NUM+1);
	nextCharIndex = 0;
	bytesReceived = FALSE;
	commandToProcess = FALSE;
}

int parseUserEntry()
{

	for (int j=0;j<TOTAL_NUMBER_COMMANDS;j++)
	{
		if (strcasecmp(userEntry, keywords[j]) == 0)


			//The plus one is because the j loop and the array starts at index 0,
			//but if we send back index o (first command in the array of commands),
			//that is going to create a problem because unknow code is also 0
			return (j+1);
	}

	return UNKNOWN_ENTRY;
}



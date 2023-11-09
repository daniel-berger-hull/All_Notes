#include "stm32f30x.h"
#include "stm32f30x_usart.h"

#include "usart2.h"

#define MIN(a, b) ((a)<(b)?(a):(b))



#define TOTAL_NUMBER_COMMANDS  5

volatile uint8_t USART2_ringbuf[RINGBUF_SIZE];
volatile uint32_t USART2_readidx = 0;
volatile uint32_t USART2_writeidx = 0;

volatile uint16_t receivedChar = 0;


volatile uint8_t userEntry[NUM+1] = {'\0'};
uint8_t keywords[TOTAL_NUMBER_COMMANDS][10] = {"run","start","stop","reset","transfer"};



volatile int commandToProcess = FALSE;
uint32_t nextCharIndex = 0;
volatile int bytesReceived = FALSE;



void USART2_Init(void)
{
		USART2_GPIO_Configuration();

		  /* enable usart clock */
		//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

			//USART_DeInit(USART1);
			USART_DeInit(USART2);

		    USART_InitTypeDef USART_InitStructure;
		    USART_StructInit(&USART_InitStructure);
		    USART_InitStructure.USART_BaudRate = 115200;
		    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		    USART_InitStructure.USART_StopBits =  USART_StopBits_1;
		    USART_InitStructure.USART_Parity =    USART_Parity_No;
		    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//	    USART_Init(USART1, &USART_InitStructure);
		    USART_Init(USART2, &USART_InitStructure);

	//	    USART_Cmd(USART1, ENABLE);
		    USART_Cmd(USART2, ENABLE);

	//		USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
			USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);


			USART2_NVIC_Configuration();
}

void USART2_GPIO_Configuration(void)
{
	GPIO_InitTypeDef      GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;  // GPIO_Pin_8 |
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_7);
	//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_7);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_7);

}


void USART2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTx Interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//NVIC_EnableIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);

}



void USART2_putc(char ch)
{
    while (1) {
        uint32_t capacity = RINGBUF_SIZE - (USART2_writeidx - USART2_readidx);
        if (capacity > 0) break;
    }
    USART2_ringbuf[(USART2_writeidx++) & (RINGBUF_SIZE-1)] = ch;
}

void USART2_write(const char *str, int len)
{
    uint32_t i = 0;
    while (i < len) {
        uint32_t writeidx = USART2_writeidx & (RINGBUF_SIZE-1);
        uint32_t len_to_end = RINGBUF_SIZE - writeidx;
        uint32_t capacity = RINGBUF_SIZE - (USART2_writeidx - USART2_readidx);
        uint32_t max_len = MIN(len_to_end, capacity);
        if (max_len == 0) continue;

        uint32_t this_len = MIN(max_len, len - i);

        int j;
        for (j = 0; j < this_len; ++j) {
            USART2_ringbuf[writeidx++] = str[i++];
        }
        USART2_writeidx += this_len;

        USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    }
}

void USART2_print(const char *str)
{
    uint32_t len = strlen(str);
    USART2_write(str, len);
}

void USART2_directputc(const char ch)
{
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    USART_SendData(USART2, ch);
}

void USART2_directprint(const char *str)
{
    uint32_t i = 0;
    uint32_t len = strlen(str);
    while (i < len) {
        USART2_directputc(str[i++]);
    }
}

void USART2_flush(void)
{
    while (USART2_readidx != USART2_writeidx);
}



void USART2_IRQHandler(void)
{
    //the transmitting used in this demo is asychronus, meaning that the MCU does not wait for the next
	//byte to be transmitted or received, but delegate the work to this interrupt method to do the
	//required work only when the appropriate flag is set in the Status Flag Register
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{

		if (USART2_writeidx - USART2_readidx == 0) {
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			return;
		}

		USART_SendData(USART2, USART2_ringbuf[(USART2_readidx++) & (RINGBUF_SIZE-1)]);
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
//		if (strcmp(entry, keywords[j]) == 0)
			if (strcmp(userEntry, keywords[j]) == 0)
			//The plus one is because the j loop and the array starts at index 0,
			//but if we send back index o (first command in the array of commands),
			//that is going to create a problem because unknow code is also 0
			return (j+1);
	}

	return UNKNOWN_ENTRY;
}



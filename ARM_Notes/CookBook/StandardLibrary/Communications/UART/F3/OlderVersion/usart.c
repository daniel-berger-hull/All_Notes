/***************************************************************************************
  BASIC USART CODE, RUNNING ON THE STM32F303VG (Discovery Board)

 Make sure to invoke the following method in your main.c

  * initGPIO();
  * initUSART();
  * NVIC_Configuration();

 A typical UART transmission will use a char array, and string manipulation
 example:

  char outputMessage[50];
  strcpy(outputMessage,"The message you want to send to the UART\n\r");

  a) USART1_directprint(outputMessage);  //-->> For a straight tranmission, no interrupt
  b) USART1_print(outputMessage); //-->For a tranmission using circular buffer and interrupt

****************************************************************************************/


#include "stm32f30x.h"
#include "stm32f30x_usart.h"

#include "usart.h"

#define MIN(a, b) ((a)<(b)?(a):(b))


volatile uint8_t USART1_ringbuf[RINGBUF_SIZE];
volatile uint32_t USART1_readidx = 0;
volatile uint32_t USART1_writeidx = 0;

volatile uint16_t receivedChar = 0;







void USART1_putc(char ch)
{
    while (1) {
        uint32_t capacity = RINGBUF_SIZE - (USART1_writeidx - USART1_readidx);
        if (capacity > 0) break;
    }
    USART1_ringbuf[(USART1_writeidx++) & (RINGBUF_SIZE-1)] = ch;
}

void USART1_write(const char *str, int len)
{
    uint32_t i = 0;
    while (i < len) {
        uint32_t writeidx = USART1_writeidx & (RINGBUF_SIZE-1);
        uint32_t len_to_end = RINGBUF_SIZE - writeidx;
        uint32_t capacity = RINGBUF_SIZE - (USART1_writeidx - USART1_readidx);
        uint32_t max_len = MIN(len_to_end, capacity);
        if (max_len == 0) continue;

        uint32_t this_len = MIN(max_len, len - i);

        int j;
        for (j = 0; j < this_len; ++j) {
            USART1_ringbuf[writeidx++] = str[i++];
        }
        USART1_writeidx += this_len;

        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
}

void USART1_print(const char *str)
{
    uint32_t len = strlen(str);
    USART1_write(str, len);
}

void USART1_directputc(const char ch)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, ch);
}

void USART1_directprint(const char *str)
{
    uint32_t i = 0;
    uint32_t len = strlen(str);
    while (i < len) {
        USART1_directputc(str[i++]);
    }
}

void USART1_flush(void)
{
    while (USART1_readidx != USART1_writeidx);
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

	/*	if  (receivedChar == RETURN_KEY)
			commandToProcess = TRUE;
		else if(nextCharIndex == NUM)
		 	resetUserEntry();
		 else
		 {
			userEntry[nextCharIndex++] = receivedChar;
			userEntry[nextCharIndex] = '\0';
			bytesReceived = TRUE;
		 } */
    }
}


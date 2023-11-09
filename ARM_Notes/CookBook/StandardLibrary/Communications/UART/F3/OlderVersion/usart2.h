/***************************************************************************************
		  BASIC USART CODE, RUNNING ON THE STM32F303VG (Discovery Board)

	Consult the code header of usart.c for more details and usage of the lib

****************************************************************************************/

#ifndef USART_H
#define USART_H

#define RINGBUF_SIZE  (1<<RINGBUF_SIZE_BITS)

#define TRUE 		 1
#define FALSE 		 0

#define NUM 		   20
#define RETURN_KEY	   13
#define UNKNOWN_ENTRY   0

#define RINGBUF_SIZE_BITS 5
void USART2_Init(void);
void USART2_GPIO_Configuration(void);
void USART2_NVIC_Configuration(void);

void USART2_IRQHandler(void);
void USART2_putc(char ch);
void USART2_write(const char *str, int len);
void USART2_print(const char *str);
void USART2_directputc(const char ch);
void USART2_directprint(const char *str);
void USART2_flush(void);

void resetUserEntry(void);
//int parseUserEntry(uint8_t entry[]);
int parseUserEntry(void);


#endif

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

/*
 *  Methods Interfacing with the USART Hardware
 */
void USART_Configuration(void);
void USART_GPIO_Configuration(void);
void USART_NVIC_Configuration(void);
void USART_putc(char ch);
void USART_write(const char *str, int len);
void USART_print(const char *str);
void USART_directputc(const char ch);
void USART_directprint(const char *str);
void USART_flush(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

/*
 *  User Commands processing methods
 */
void resetUserEntry(void);
int  parseUserEntry(void);


#endif

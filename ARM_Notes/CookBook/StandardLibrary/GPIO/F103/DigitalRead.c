/**
  *****************************************************************************
  * @title        DigitalRead.c
  * @Description  Show how to Read a bit on the GPIO. For this
  * 			  Example, the pin 3 of Port A will be read
  *
 *   Steps:
 *   1) Activate the clock on the required port (here port A)
 *      The function *** RCC_APB2PeriphClockCmd ***    is used
 *      Note: All the GPIO port are on the APB2 section of the Bus

 *    2) Configure the Port using the GPIO_InitTypeDef
 *       You have to set:
 *       i)  The pin number       --> Here pin 0 to,              value GPIO_Pin_0 (0x1) to GPIO_Pin_3 (0x8)
 *       ii) The action           --> Here a floating input       value GPIO_Mode_IN_FLOATING
 *           Note: There are at least 2 or 3 other type of input on the STM32F103,
 *                 and you may want to explore mode. But the floating one appears to
 *                 work just fine...
 *       iii) The Pin Clock Speed --> 50 Mhz is used,             value GPIO_Speed_50MHz
 *
 *    3) Read the value on the pin targeted
 *       Many function exist, but *** GPIO_WriteBit *** work well
 *       Have to indicate which GPIO port, which pin and which state (Enable/Reset)
  ********************************************************************************/


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


int main(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	//Make sure to put the clock active on the Port A of the GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//Configure the pins as output, running at 50 MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	printf("Staring reading pin...\r");

	while(1){

 		uint8_t state =  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
		printf("Value is %d\r", state);
	}
}

/*******************************************************************************
 * @title        DigitalWrite.c
 * @Description  Show how to Write a bit on the GPIO. For this
 * 			     Example, the Port A bit 0 to 3 will be used
 *
 *   Steps:
 *   1) Activate the clock on the required port (here port A)
 *      The function *** RCC_APB2PeriphClockCmd ***    is used
 *      Note: All the GPIO port are on the APB2 section of the Bus
 *
 *    2) Configure the Port using the GPIO_InitTypeDef
 *       You have to set:
 *       i)  The pin number       --> Here pin 0 to,      value GPIO_Pin_0 (0x1) to GPIO_Pin_3 (0x8)
 *       ii) The action           --> Here an output,     value GPIO_Mode_Out_PP
 *       iii) The Pin Clock Speed --> 50 Mhz is used,     value GPIO_Speed_50MHz
 *
 *    3) Write the value on the bit targeted
 *       Many functions exist, but *** GPIO_WriteBit *** works well
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
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);



	while(1){
		GPIO_WriteBit(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ,Bit_SET);

		for (int i=0;i<1000000;i++);

		GPIO_WriteBit(GPIOA,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ,Bit_RESET);
		for (int i=0;i<1000000;i++);
	}
}

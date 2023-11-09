/**
  *****************************************************************************
  * @title        BlinkLed.c
  * @Description  Show how to blink the on board led (User Led) of the STM32F103C8t6 mini board.
  * 		      The User Led is connected to the pin #13 of the GPIO port C, and is inverted.
  * 		      So to light on  --> Value 0
  * 		         to turn off  --> Value 1
  *
 *   Steps:
 *   1) Activate the clock on the required port (here port C)
 *      The function *** RCC_APB2PeriphClockCmd ***    is used
 *      Note: All the GPIO port are on the APB2 section of the Bus

 *    2) Configure the Port using the GPIO_InitTypeDef
 *       You have to set:
 *       i)  The pin number       --> Here pin 13 of the GPIO port C
 *       ii) The action           --> Here an output,     value GPIO_Mode_Out_PP
 *       iii) The Pin Clock Speed --> 50 Mhz is used,     value GPIO_Speed_50MHz
 *
 *    3) Write the value on the pin to control the state of the user led.
  ********************************************************************************/

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>


void InitializeLEDs()
{
	GPIO_InitTypeDef gpioStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpioStructure.GPIO_Pin =  GPIO_Pin_13;
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpioStructure);
}


int main(void)
{
	InitializeLEDs();

    while(1)
    {
    	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    	for (int i=0;i<5000000;i++);
    	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    	for (int i=0;i<5000000;i++);
    }
}

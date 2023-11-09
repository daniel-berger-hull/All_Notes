/**
  *****************************************************************************
  * @title        BlinkLed.c
  * @Description  Show how to blink any User led of the STM32F3 Discoboard mini board.
  * 		      The User Leds are connected to the  GPIO port E, from pin 8 up to 15.
  *
  *
  * 		         GPIO_Pin_8  -->      Blue LED top left
  * 			     GPIO_Pin_9  -->      Red LED top
  * 				 GPIO_Pin_10 -->      Orange LED top right
  * 				 GPIO_Pin_11 -->      Green LED right
  * 				 GPIO_Pin_12 -->      Blue LED bottom right
  * 				 GPIO_Pin_13 -->      Red LED bottom
  * 				 GPIO_Pin_14 -->      Orange LED bottom left
  *					 GPIO_Pin_15 -->      Green LED left
  *
  *
  * 		To Turn ON or OFF, be carefull with the field GPIO_PuPd of the the gpioStructure structure
  * 		So, if gpioStructure.GPIO_PuPd = GPIO_PuPd_UP      pin #13 of the, and is inverted.
  * 		      So to light on  --> Value Bit_SET
  * 		         to turn off  --> Value Bit_RESET
  *
  *   Steps:
 *   1) Activate the clock on the required port (here port E)
 *      The function *** RCC_AHBPeriphClockCmd ***    is used
 *      Note: For the F303, all the GPIO ports appear to be on the high speed bus *** AHB ***,
 *      and not the peripheral bus like APB1 or APB2, like it is the case for the F103...
 *
 *    2) Configure the Port using the GPIO_InitTypeDef
 *       You have to set:
 *       i)   The pin number       --> Here any pin from 8 to 15 on the the GPIO port E
 *       ii)  The action           --> Here an output,     value GPIO_Mode_OUT
 *       iii) The Pin Clock Speed --> 50 Mhz is used,     value GPIO_Speed_50MH
 *       iv)  The Pull UP or Pull --> GPIO_PuPd_UP for a Pull up
 *       							  GPIO_PuPd_DOWN for a Pull down
 *
 *    3) Write the value on the pin to control the state of the user led.
  ********************************************************************************/

#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>


void InitializeLEDs()
{
	GPIO_InitTypeDef gpioStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

    gpioStructure.GPIO_Pin =  GPIO_Pin_8 |     //Blue LED top left
    					      GPIO_Pin_9 |     //Red LED top
    					      GPIO_Pin_10 |    //Orange LED top right
    					      GPIO_Pin_11 |    //Green LED right
    					      GPIO_Pin_12 |    //Blue LED bottom right
    					      GPIO_Pin_13 |    //Red LED bottom
    					      GPIO_Pin_14 |   //Orange LED bottom left
    					      GPIO_Pin_15; ;  //Green LED left
    gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
    gpioStructure.GPIO_PuPd = GPIO_PuPd_UP;  //This will pull up the pin, so if we:
    										 // i)  Write a Bit_SET   (value 1) --> LED will Light ON
    										 // ii) Write a Bit_RESET (value 0) --> Turn of the LED

    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &gpioStructure);
}


int main(void)
{
	InitializeLEDs();

    while(1)
    {
    	GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET);   //Can set any pin from 8 to  15
    	for (int i=0;i<8000000;i++);
    	GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);  //Can reset any pin from 8 to  15
    	for (int i=0;i<8000000;i++);
    }
}

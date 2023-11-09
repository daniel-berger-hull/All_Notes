/*******************************************************************************
  * @title        GraphicDemo.c
  * @Description  Show few graphic function of the monochrome LCD Nokia 5110.
  *				  The lib manage to find the right memory location in the LCD
  *				  to acheive to required drawing
  *
  * Connection with the STM32F103C8T6:
  *                  STM32			Nokia 5110
  *				    -------------------------------
  *   				  A12 	 -->    LCD_PIN_ENABLE
  *   				  A11 	 -->    LCD_PIN_RESET
  *   				  A10 	 -->    LCD_PIN_COMMAND
  *   				  A9	 -->    LCD_PIN_DATA
  *   				  A8	 -->    LCD_PIN_CLOCK
  *					 GND     -->    LCD_LIGHT
  *
  *		Note: Put the LCD_LIGHT to the ground to make it light up permenantly...
  *
  ********************************************************************************/


#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "Nokkia5110.h"


GPIO_InitTypeDef GPIO_InitStructure;

void initPins()
{
		// Initialize PA8-PA12 as outputs
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin = LCD_PIN_ENABLE | LCD_PIN_RESET | LCD_PIN_COMMAND | LCD_PIN_DATA | LCD_PIN_CLOCK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

int main(void)
{
	initPins();

	// Reset LCD & Initialise...
	LcdReset();
	LcdInitalise();

	 Lcd_clear_screen();
	 Lcd_Draw_Line(0,0,83,47);
	 Lcd_Draw_Rectangle(2, 2, 81, 45, OFF);
	 Lcd_Draw_Circle(40,20,15,OFF);

	while (1)
	{
	}
}


/******************************************************************************************
  * @title        TextDemo.c
  * @Description  Show how to write test on the screen of the  monochrome LCD Nokia 5110.
  *				  The lib has 2 distinct way to print:
  *				       @ Char by Char -->  Lcd_print_char
  *				       @ String       -->  Lcd_print_string
  *
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
  *******************************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "Nokkia5110.h"


GPIO_InitTypeDef GPIO_InitStructure;

/*
uint8_t messageBytes[] = {
	0x3e, 0x41, 0x41, 0x41, 0x22, 0x00, 0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x7c, 0x08, 0x04, 0x04, 0x08, 0x00,
	0x04, 0x3f, 0x44, 0x40, 0x20, 0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00, 0x44, 0x28, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00, 0x21, 0x41, 0x45, 0x4b, 0x31, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x01, 0x01, 0x7f, 0x01, 0x01, 0x00, 0x7f, 0x02, 0x0c, 0x02, 0x7f, 0x00,
	0x21, 0x41, 0x45, 0x4b, 0x31, 0x00, 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x7f, 0x41, 0x41, 0x22, 0x1c, 0x00,
	0x00, 0x44, 0x7d, 0x40, 0x00, 0x00, 0x48, 0x54, 0x54, 0x54, 0x20, 0x00, 0x38, 0x44, 0x44, 0x44, 0x20, 0x00,
	0x38, 0x44, 0x44, 0x44, 0x38, 0x00, 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x00, 0x38, 0x54, 0x54, 0x54, 0x18, 0x00,
	0x7c, 0x08, 0x04, 0x04, 0x08, 0x00, 0x0c, 0x50, 0x50, 0x50, 0x3c, 0x00
}; */


uint8_t graphic [] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
	0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
	0xC0, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC, 0xF8, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFE,
	0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xC0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
	0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFE, 0xFC, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F, 0x1F,
	0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
	0x7F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x03,
	0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

uint8_t mikro_bmp[] = {  //504
  0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255,   7,  71,  99,  99,  99,
 99,  99,  99,  99, 103,   7, 143, 255, 255,  63,  63,  31,
 15,  15,   7,   7,   7,   3,   3,   3,   3,   3,   3,   3,
  3,   3,   7,   7,   7,  15,  15,  31,  31,  63, 255, 255,
255, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255,  14,  12,  12,  28,  60,
 60,  60,  60,  60, 252, 254, 255,   3,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0, 248, 254, 254, 254, 252,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
 15, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255,  34, 102,   6,   6,  30,
 14,   6,   6,  34, 255, 255, 255,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   1,   3,   3,   3,   3,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255, 196, 254, 222, 198, 198,
198, 198, 198, 198, 255, 255, 255,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0, 254, 255, 255, 255, 255,
255,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,   7,
  7, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255,  16,  24,  24,  24,  24,
 24,  24,  24,  24,  27,  63, 255, 224, 128,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,  15,  63, 127,  63,  63,
 15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,
240, 255, 255, 255, 255,   0, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0, 255, 255,   0, 255, 255, 255, 224, 227, 227, 227, 227,
227, 227, 227, 227, 227, 227, 227, 227, 231, 238, 252, 248,
248, 240, 240, 224, 224, 224, 224, 224, 224, 224, 224, 224,
224, 224, 224, 224, 224, 240, 240, 248, 252, 254, 255, 255,
255, 255, 255, 255, 127, 128, 255, 255,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};


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


	 unsigned char g = 0;
	 const unsigned char txt1[11] = {"MicroArena"};


	while (1)
	{
		 //Will write char by char, using the function Lcd_print_char
		Lcd_clear_screen();
		 for(g = 0; g <= 9; g++)
	     {
			 	Lcd_set_cursor((18 + (g * 5)), 3);
			 	Lcd_print_char(txt1[g]);
	      }

		 delay_ms(2500);

		 //Will use the string printing, which is more convenient than
		 //writing char by char...
		 Lcd_clear_screen();
		 Lcd_print_string(0, 0, "Writing...");
		 Lcd_print_string(0, 1, "Daniel");
		 Lcd_print_string(0, 2, "Berger");
		 Lcd_print_string(3, 3, "1971/03/05");
		 Lcd_print_string(0, 4, "44 De La Chimere");
		 Lcd_print_string(4, 5, "Gatineau");
		 delay_ms(2500);
	}
}


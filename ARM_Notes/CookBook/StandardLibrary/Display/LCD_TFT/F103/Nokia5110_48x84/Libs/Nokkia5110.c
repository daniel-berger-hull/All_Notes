/**
  *****************************************************************************
  * @title        Nokkia5110.c
  * @Description  Simple Driver lib for the  monochrome LCD Nokia 5110.
  *               Add Nokkia5110.h and Nokkia5110.c to your project to have the
  *               display function into your project
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


void LcdWrite(uint8_t dataOrCommand, uint8_t value)
{
	if (dataOrCommand)
		GPIO_SetBits(GPIOA, LCD_PIN_COMMAND);
	else
		GPIO_ResetBits(GPIOA, LCD_PIN_COMMAND);

	GPIO_ResetBits(GPIOA, LCD_PIN_ENABLE);       // enable LCD

	int i;
	for (i = 0; i < 8; i++)
	{
		GPIO_ResetBits(GPIOA, LCD_PIN_CLOCK);
		if (value & 0x80)
			GPIO_SetBits(GPIOA, LCD_PIN_DATA);
		else
			GPIO_ResetBits(GPIOA, LCD_PIN_DATA);
		value <<= 1;
		GPIO_SetBits(GPIOA, LCD_PIN_CLOCK);      // LCD latches data on the rising clock edge
	}

	GPIO_SetBits(GPIOA, LCD_PIN_ENABLE);         // disable LCD
}

/*
 *   The LCD will reset by simpling setting the RST pin, wait (here 10 mSec, but some code take a shorter time)
 *   and bring the pin back to the ground...
 */

void LcdReset()
{
	GPIO_SetBits(GPIOA, LCD_PIN_RESET);
	delay_ms(10);
	GPIO_ResetBits(GPIOA, LCD_PIN_RESET);
	delay_ms(10);
	GPIO_SetBits(GPIOA, LCD_PIN_RESET);
}


void LcdInitalise()
{
	// Initialize LCD state
	LcdWrite(LCD_CMD, 0x21); // LCD Extended Commands.
	LcdWrite(LCD_CMD, 0xB1); // Set LCD Vop (Contrast).
	LcdWrite(LCD_CMD, 0x04); // Set Temp coefficent.
	LcdWrite(LCD_CMD, 0x15); // LCD bias mode
	LcdWrite(LCD_CMD, 0x20);
	LcdWrite(LCD_CMD, 0x0C); // LCD in normal mode.
}

void Lcd_set_cursor(uint8_t x_pos, uint8_t y_pos)
{
	//

	LcdWrite(LCD_CMD, (PCD8544_set_X_addr | x_pos));
	LcdWrite(LCD_CMD, (PCD8544_set_Y_addr | y_pos));
}

void Lcd_print_char(uint8_t ch)
{
	 uint8_t s = 0;
	uint8_t chr = 0;

     for(s = 0; s <= 4; s++)
     {
           chr = font[(ch - 0x20)][s];
           LcdWrite(LCD_DATA, chr);
     }
}


void Lcd_print_string(uint8_t x_pos, uint8_t y_pos, uint8_t *ch)
{
	Lcd_set_cursor(x_pos, y_pos);

    do
    {
    	Lcd_print_char(*ch++);
    }while((*ch >= 0x20) && (*ch <= 0x7F));
}


void Lcd_clear_screen()
{
	uint8_t x_pos = 0;
	uint8_t y_pos = 0;

    for(y_pos = 0; y_pos < Rows; y_pos++)
    {
        for(x_pos = 0; x_pos < X_max; x_pos++)
        {
        	Lcd_print_string(x_pos, y_pos, " ");
        }
    }
}

void Lcd_Draw_Pixel(uint8_t x_pos, uint8_t y_pos)
{
	uint8_t row = 0;
	uint8_t value = 0;

    if((x_pos < 0) || (x_pos >= X_max) || (y_pos < 0) || (y_pos >= Y_max))
    {
        return;
    }

    row = (y_pos >> 3);

    value = PCD8544_buffer[x_pos][row];
    value |= (1 << (y_pos % 8));
    PCD8544_buffer[x_pos][row] = value;

    Lcd_set_cursor(x_pos, row);
    LcdWrite(DAT, value);
}


void Lcd_Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2)
{
    signed int dx = 0x0000;
    signed int dy = 0x0000;
    signed int stepx = 0x0000;
    signed int stepy = 0x0000;
    signed int fraction = 0x0000;

    dy = (y2 - y1);
    dx = (x2 - x1);

    if (dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if (dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 0x01;
    dy <<= 0x01;

    Lcd_Draw_Pixel(x1, y1);

    if (dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while (x1 != x2)
        {
            if (fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            Lcd_Draw_Pixel(x1, y1);
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));

        while (y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            Lcd_Draw_Pixel(x1, y1);
        }
    }
}



void Lcd_Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, uint8_t fill)
{
	 uint8_t i = 0x00;
     uint8_t xmin = 0x00;
     uint8_t xmax = 0x00;
     uint8_t ymin = 0x00;
     uint8_t ymax = 0x00;

     if(fill != NO)
     {
        if(x1 < x2)
        {
           xmin = x1;
           xmax = x2;
        }
        else
        {
           xmin = x2;
           xmax = x1;
        }

        if(y1 < y2)
        {
           ymin = y1;
           ymax = y2;
        }
        else
        {
           ymin = y2;
           ymax = y1;
        }

        for(; xmin <= xmax; ++xmin)
        {
             for(i = ymin; i <= ymax; ++i)
             {
            	  Lcd_Draw_Pixel(xmin, i);
             }
         }
     }

     else
     {

    	 Lcd_Draw_Line(x1, y1, x2, y1);
    	 Lcd_Draw_Line(x1, y2, x2, y2);
    	 Lcd_Draw_Line(x1, y1, x1, y2);
    	 Lcd_Draw_Line(x2, y1, x2, y2);
     }
}


void Lcd_Draw_Circle(signed int xc, signed int yc, signed int radius, uint8_t fill)
{
    signed int a = 0x0000;
    signed int b = 0x0000;
    signed int p = 0x0000;

    b = radius;
    p = (1 - b);

    do
    {
        if(fill != NO)
        {
        	Lcd_Draw_Line((xc - a), (yc + b), (xc + a), (yc + b));
        	Lcd_Draw_Line((xc - a), (yc - b), (xc + a), (yc - b));
        	Lcd_Draw_Line((xc - b), (yc + a), (xc + b), (yc + a));
        	Lcd_Draw_Line((xc - b), (yc - a), (xc + b), (yc - a));
        }
        else
        {
        	Lcd_Draw_Pixel((xc + a), (yc + b));
        	Lcd_Draw_Pixel((xc + b), (yc + a));
        	Lcd_Draw_Pixel((xc - a), (yc + b));
        	Lcd_Draw_Pixel((xc - b), (yc + a));
        	Lcd_Draw_Pixel((xc + b), (yc - a));
        	Lcd_Draw_Pixel((xc + a), (yc - b));
        	Lcd_Draw_Pixel((xc - a), (yc - b));
        	Lcd_Draw_Pixel((xc - b), (yc - a));
        }

        if(p < 0)
        {
            p += (0x03 + (0x02 * a++));
        }
        else
        {
            p += (0x05 + (0x02 * ((a++) - (b--))));
        }
    }while(a <= b);
}


void delay_ms(__IO uint32_t nCount)
{
	uint32_t delay = nCount * 2516;              // approximate loops per ms at 24 MHz, Debug config
	for(; delay != 0; delay--);
}

/******************************************************************************************************
 *  This code is a C language driver for the LCDs having a HD44780 controller chip.
 *  The code assume that an STM32F103 MCU is used (STM32F103C8T6 been used for the testing)
 *
 * The following pin layout been used
 *       LCD          STM32     Color
 *     ------------------------------------
 *       D4            A4		Green
 *       D5            A5		Blue
 *       D6            A6		Purple
 *       D7            A7		Gray
 *       RS			   B10		Black
 *       EN			   B11		White
 *
 *  The RW pin of the LCD is grounded, so we can only write command or Data to the LCD
 *
 *    Note 1: Make sure that the LCD is powered by a power supply, not the STM32
 *            The STM32 has only 3.3 V, is the display is way too light to be visible
 *            That way, you can put a 5 V to power the LCD, and the image is bright and stable
 *
 *    Note 2: Make sure that all devices share a ground line (LCD & STM32)
 *            The signal been not transmitted in early test due to this
 *
 *    Note 3: 2 different GPIO ports used for this demo code (Port A for Data, Port B for Control)
 *            The reason is that I suspect that there is a limit to the power a single port can sink,
 *            and driving all the Data pins plus the control pins may have contributed to the initial
 *            failure of the LCD tests.
 *
 ********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "lcd_hd44780.h"
#include "stm32_delay.h"



GPIO_InitTypeDef GPIO_InitStructure;

unsigned int lcdMemOffset[LCD_NBR_LINES] = {0x00,0x40,0x14,0x54};


void Toggle_EnablePin()
{
   GPIO_WriteBit(LCD_GPIO_CTRL, LCD_EN, Bit_SET);
   Delay_us(20);
   GPIO_WriteBit(LCD_GPIO_CTRL, LCD_EN, Bit_RESET);
   Delay_us(20);
}


//-----------------------------------------------------------------------------
void lcd_writenibble(unsigned char nibbleToWrite)
{
  GPIO_WriteBit(LCD_GPIO_DATA, LCD_D4,(BitAction) (nibbleToWrite & 0x01));
  GPIO_WriteBit(LCD_GPIO_DATA, LCD_D5,(BitAction) (nibbleToWrite & 0x02));
  GPIO_WriteBit(LCD_GPIO_DATA, LCD_D6,(BitAction) (nibbleToWrite & 0x04));
  GPIO_WriteBit(LCD_GPIO_DATA, LCD_D7,(BitAction) (nibbleToWrite & 0x08));
  Toggle_EnablePin();
}




//-----------------------------------------------------------------------------
void lcd_writedata(unsigned char dataToWrite)
{
  GPIO_WriteBit(LCD_GPIO_CTRL, LCD_RS, Bit_SET);

  lcd_writenibble(dataToWrite >> 4);
  lcd_writenibble(dataToWrite & 0x0F);
}


//-----------------------------------------------------------------------------
void lcd_writecommand(unsigned char commandToWrite)
{
  GPIO_WriteBit(LCD_GPIO_CTRL, LCD_RS, Bit_RESET); // LCD_RW |
  lcd_writenibble(commandToWrite >> 4);
  lcd_writenibble(commandToWrite & 0x0F);
  Delay_ms(1);
}


//-----------------------------------------------------------------------------
void lcd_str(unsigned char * text)
{
  while(*text)
    lcd_writedata(*text++);
}


//-----------------------------------------------------------------------------
void lcd_locate(unsigned char x, unsigned char y)
{
	if (y < LCD_MIN_ROW ||  y  > LCD_MAX_ROW)
		return;

	if (x < LCD_MIN_COL ||  x  > LCD_MAX_COL)
		return;

	lcd_writecommand(HD44780_DDRAM_SET | (x + lcdMemOffset[y]));
}


//-----------------------------------------------------------------------------
void lcd_strxy(unsigned char * text, unsigned char x, unsigned char y)
{
  lcd_locate(x,y);
  while(*text)
    lcd_writedata(*text++);
}


//-----------------------------------------------------------------------------
void lcd_writebinary(unsigned int var, unsigned char bitCount)
{
  signed char i;

  for(i = (bitCount - 1); i >= 0; i--)
     {
     lcd_writedata((var & (1 << i))?'1':'0');
     }
}


//-----------------------------------------------------------------------------
void LCD_ShiftLeft(void)
{
  lcd_writecommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_LEFT | HD44780_SHIFT_DISPLAY);
}


//-----------------------------------------------------------------------------
void LCD_ShiftRight(void)
{
  lcd_writecommand(HD44780_DISPLAY_CURSOR_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_DISPLAY);
}


/**
 * The initialization sequence of the HD44780 is the following:
 *
 * 1) Put the Data Port (LCD's D4 to D7) to low (0 Volt)
 * 2) Wait 40 mSec for the LCD to be ready
 * 3) Init the 4 bits mode by
 * 		3.1) Write value 0x03 to Data Port
 *		3.2) Wait 4.5 mSec
 * 		3.2) Write value 0x03 to Data Port
 * 		3.3) Wait 4.5 mSec
 * 		3.4) Write value 0x03 to Data Port
 * 		3.5) Wait 4.5 mSec
 * Note: You have wrote 3 times the value 0x03 to the Data Port
 * 		3.6) Write value 0x02 to Data Port
 * 		3.7) Wait 0.1 mSec (Don't have to wait as long as the other 3 previous times)
 *  4) Send the Configuration Command
 *    4BITMODE (Value 0x00) + 5x8DOTS (Value 0x00) + 2 Display LINE (0x08)
 *  4.1)  Wait 3 mSec
 *  5) Sent the Display On Command (Value 0x04)
 *  5.1)Wait 3 mSec
 *  Facultative (but make display cleaner)
 *  6) Send the Entry Configuration Command
 *   ENTRY To the LEFT  (Value 0x02) + Shift Decrement (Value 0x00)
 *  6.1)Wait 3 mSec
**/
void lcd_init(void)
{
  volatile unsigned char i = 0;
  volatile unsigned int delayCnt = 0;

  RCC_APB2PeriphClockCmd(LCD_DATA_LINE, ENABLE);
  RCC_APB2PeriphClockCmd(LCD_CRTL_LINE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LCD_GPIO_DATA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LCD_RS | LCD_EN;  //LCD_RW|
  GPIO_Init(LCD_GPIO_CTRL, &GPIO_InitStructure);

  GPIO_ResetBits(LCD_GPIO_DATA,  LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
  GPIO_ResetBits(LCD_GPIO_CTRL, LCD_RS | LCD_EN );


  // according to datasheet, we need at least 40-45ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  Delay_ms(45);

  for(i = 0; i < 3; i++) {
    lcd_writenibble(0x03);
    Delay_ms(5);
  }

  lcd_writenibble(0x02);
  Delay_us(100);

  lcd_writecommand(HD44780_FUNCTION_SET |
                   HD44780_FONT5x7 |
                   HD44780_TWO_LINE |
                   HD44780_4_BIT);

  lcd_writecommand(HD44780_DISPLAY_ONOFF |
		  	  	   HD44780_DISPLAY_ON);

  lcd_writecommand(HD44780_CLEAR);

  lcd_writecommand(HD44780_ENTRY_MODE |
                   HD44780_EM_SHIFT_CURSOR |
                   HD44780_EM_INCREMENT);

  lcd_writecommand(HD44780_DISPLAY_ONOFF |
                   HD44780_DISPLAY_ON |
                   HD44780_CURSOR_OFF |
                   HD44780_CURSOR_NOBLINK);

}


//-----------------------------------------------------------------------------
void lcd_addchar (unsigned char chrNum, unsigned char n, const unsigned char *p)
{
        lcd_writecommand(HD44780_CGRAM_SET | chrNum * 8);
        n *= 8;
        do
                lcd_writedata(*p++);
        while (--n);
}


//-----------------------------------------------------------------------------
void lcd_cls(void){
        lcd_writecommand(HD44780_CLEAR);
}



unsigned char* intToStr(int n)
        {
     int i = 0;
     char *tmp = (char*)malloc(sizeof(char));
     unsigned char *ret = (unsigned char*)malloc(12);
     if(n < 0) {
          *ret = '-';
          i++;
          n = -n;
     }
     do {
          *tmp = n % 10 + 48;
          n -= n % 10;
          if(n > 9) *tmp++;
     }
     while(n /= 10);
     while(ret[i++] = *tmp--);
     return ret;
}

void lcd_int(int n){

        lcd_str(intToStr(n));
}

void lcd_intxy(int n, unsigned char x, unsigned char y){
        lcd_locate(x,y);
        lcd_str(intToStr(n));
}

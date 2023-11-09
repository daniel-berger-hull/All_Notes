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
#include "stm32f10x.h"


#define LCD_GPIO_DATA GPIOA
#define LCD_GPIO_CTRL GPIOB

#define LCD_DATA_LINE RCC_APB2Periph_GPIOA
#define LCD_CRTL_LINE RCC_APB2Periph_GPIOB

#define LCD_D4 GPIO_Pin_4
#define LCD_D5 GPIO_Pin_5
#define LCD_D6 GPIO_Pin_6
#define LCD_D7 GPIO_Pin_7

#define LCD_RS GPIO_Pin_10
#define LCD_EN GPIO_Pin_11

#define LCD_NBR_LINES  4
#define LCD_MIN_ROW    0
#define LCD_MAX_ROW    3
#define LCD_MIN_COL    0
#define LCD_MAX_COL    20


//******************************************************************************//


#define HD44780_CLEAR                           0x01
#define HD44780_HOME                            0x02

#define HD44780_ENTRY_MODE                      0x04
#define HD44780_EM_SHIFT_CURSOR          		0
#define HD44780_EM_SHIFT_DISPLAY         		1
#define HD44780_EM_DECREMENT               		0
#define HD44780_EM_INCREMENT               		2

#define HD44780_DISPLAY_ONOFF                   0x08
#define HD44780_DISPLAY_OFF                     0
#define HD44780_DISPLAY_ON                      4
#define HD44780_CURSOR_OFF                      0
#define HD44780_CURSOR_ON                       2
#define HD44780_CURSOR_NOBLINK     				0
#define HD44780_CURSOR_BLINK                    1

#define HD44780_DISPLAY_CURSOR_SHIFT 			0x10
#define HD44780_SHIFT_CURSOR                    0
#define HD44780_SHIFT_DISPLAY                   8
#define HD44780_SHIFT_LEFT                      0
#define HD44780_SHIFT_RIGHT                     4

#define HD44780_FUNCTION_SET                    0x20
#define HD44780_FONT5x7                         0
#define HD44780_FONT5x10                        4
#define HD44780_ONE_LINE                        0
#define HD44780_TWO_LINE                        8
#define HD44780_4_BIT                           0
#define HD44780_8_BIT                           16

#define HD44780_CGRAM_SET                       0x40
#define HD44780_DDRAM_SET                       0x80

//##############################################################
void lcd_init(void);
void lcd_cls(void);
void lcd_str(unsigned char * text);
void lcd_strxy(unsigned char * text, unsigned char x, unsigned char y);
void lcd_locate(unsigned char x, unsigned char y);
void lcd_int(int n);
void lcd_intxy(int n, unsigned char x, unsigned char y);

//###############################################################

void lcd_writedata(unsigned char dataToWrite);
void lcd_writecommand(unsigned char commandToWrite);
void lcd_writebinary(unsigned int var, unsigned char bitCount);
void lcd_addchar (unsigned char chrNum, unsigned char n, const unsigned char *p);

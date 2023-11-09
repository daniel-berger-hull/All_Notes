/**
  *****************************************************************************
  * @title        Nokkia5110.h
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

#include "font.c"

#define LCD_PIN_ENABLE      GPIO_Pin_12
#define LCD_PIN_RESET       GPIO_Pin_11
#define LCD_PIN_COMMAND     GPIO_Pin_10
#define LCD_PIN_DATA        GPIO_Pin_9
#define LCD_PIN_CLOCK       GPIO_Pin_8

#define LCD_CMD             0
#define LCD_DATA            1





#define BL_pin                                                                           0
#define RST_pin                                                                          5
#define CE_pin                                                                           6
#define DC_pin                                                                           7
#define SDO_pin                                                                          8
#define SCK_pin                                                                          9

#define PCD8544_set_Y_addr                                                               0x40
#define PCD8544_set_X_addr                                                               0x80

#define PCD8544_set_temp                                                                 0x04
#define PCD8544_set_bias                                                                 0x10
#define PCD8544_set_VOP                                                                  0x80

#define PCD8544_power_down                                                               0x04
#define PCD8544_entry_mode                                                               0x02
#define PCD8544_extended_instruction                                                     0x01

#define PCD8544_display_blank                                                            0x00
#define PCD8544_display_normal                                                           0x04
#define PCD8544_display_all_on                                                           0x01
#define PCD8544_display_inverted                                                         0x05

#define PCD8544_function_set                                                             0x20
#define PCD8544_display_control                                                          0x08

#define CMD                                                                              0
#define DAT                                                                              1

#define X_max                                                                            84
#define Y_max                                                                            48
#define Rows                                                                             6

#define BLACK                                                                            0
#define WHITE                                                                            1
#define PIXEL_XOR                                                                        2

#define ON                                                                               1
#define OFF                                                                              0

#define NO                                                                               0
#define YES                                                                              1

#define buffer_size                                                                      504

unsigned char PCD8544_buffer[X_max][Rows];


void LcdWrite(uint8_t dataOrCommand, uint8_t value);
void LcdReset();
void LcdInitalise();
void Lcd_set_cursor(uint8_t x_pos, uint8_t y_pos);
void Lcd_print_char(uint8_t ch);
void Lcd_print_string(uint8_t x_pos, uint8_t y_pos, uint8_t *ch);
void Lcd_clear_screen();

void Lcd_Draw_Pixel(uint8_t x_pos, uint8_t y_pos);
void Lcd_Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2);
void Lcd_Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, uint8_t fill);
void Lcd_Draw_Circle(signed int xc, signed int yc, signed int radius, uint8_t fill);

void delay_ms(__IO uint32_t nCount);

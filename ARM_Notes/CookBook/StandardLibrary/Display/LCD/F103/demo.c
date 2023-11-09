/******************************************************************************************************
 *  This a demo code to use with the lcd_hd44780 LCD driver (HD44780 controller chip).
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

/*
 *  TO DO:
 *  ------
 *   - Use a MCU Clock or Timer for the delay in the LCD, instead of a for loop
 */
#include <stddef.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "lcd_hd44780.h"
#include "stm32_delay.h"


GPIO_InitTypeDef GPIO_InitStructure;

int main(void)
{
	lcd_init();

	while (1)
	{
		lcd_locate(0,0);
		lcd_str("Top Left");
		lcd_locate(9,1);
		lcd_str("Second Line");
		for(int delayCnt = 0; delayCnt < 10000000; delayCnt++);

		lcd_locate(0,2);
		lcd_str("Third Line");
		lcd_locate(9,3);
		lcd_str("Fourth Line");
		for(int delayCnt = 0; delayCnt < 10000000; delayCnt++);

		lcd_cls();
	}
}




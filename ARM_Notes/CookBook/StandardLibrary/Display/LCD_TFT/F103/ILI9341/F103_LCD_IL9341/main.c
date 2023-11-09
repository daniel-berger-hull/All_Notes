#include <stdio.h>

#include "lib/ILI9341/text.h"
#include "lib/tinystd/itoa.h"
#include "lib/encoder/encoder.h"



int main(void) {

	usartInit();
	usartPrintLn("Initialisation...");
    LCD_init();
    ENC_init();
    LCD_setOrientation(ORIENTATION_PORTRAIT);
    LCD_fillScreen(BLACK);

    char buffer[40];
    LCD_setTextBgColor(BLACK);
    LCD_setTextSize(3);
    u16 ov = 111;
    u16 encVal;

    uint16_t counterIn = 0;
    uint16_t counterOut = 0;
    uint16_t counterDisplay = 0;



    usartPrintLn("Starting main loop...");


    while (1) {
        if (ov != (encVal = ENC_getValue())) {
        	TEST_fillPrimitives();
        	//TEST_fill();
/*            LCD_setCursor(0, 0);
            itoa(encVal, buf, 10);
            LCD_fillRect(0, 0, 100, 24, BLACK);
            //LCD_writeString(buf);
           LCD_writeString("Daniel"); */

            ov = encVal;
            counterIn++;
        } else
        	counterOut++;

        if (++counterDisplay> 50) {
        	counterDisplay = 0;


        	 //itoa(counterOut, buf, 10);
        	sprintf(buffer,"counterOut = %d, counterIn = %d",counterOut,counterIn);
        	 usartPrintLn(buffer);
        }

        delay_ms(10);
    }

   // while (1);
}

void test() {

}


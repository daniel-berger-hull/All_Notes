
/*******************************************************************************

								DELAY FUNCTION

	Here 2 examples of very simple delay method, that decrements a counter
	till the CPU elapsed the time to wait.

	Taken from the following web page:

			http://stm32f4-discovery.com/2014/09/precise-delay-counter/


	Usage:
		 1) Declare the variable multiplier (uint32_t)
	     2) Call Method TM_Delay_Init()
	     3) Invoke the method TM_DelayMicros OR TM_DelayMillis

*************************************************************************/

uint32_t multiplier;

void TM_Delay_Init(void) {
    RCC_ClocksTypeDef RCC_Clocks;

    /* Get system clocks */
    RCC_GetClocksFreq(&RCC_Clocks);

    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M */
    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

void TM_DelayMicros(uint32_t micros) {
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}


void TM_DelayMillis(uint32_t millis) {
    /* Multiply millis with multipler */
    /* Substract 10 */
    millis = 1000 * millis * multiplier - 10;
    /* 4 cycles for one loop */
    while (millis--);
}


int main(void) {

	while (1) {
	    /* Do Anything... */
	    /* Delay 1000 micros */
	    TM_DelayMicros(1000);
	}
}

/*
*   Fast PWM Mode 3. Each channels is controlled by the value in their respective OCR register
*				Channel A --> OCR2A --> Output on Pin PB3
*				Channel B --> OCR2B --> Output on Pin PD3
*
*   The Fast PMW mode 3 has a period of 0xFF (Maximum value of the counter), which is the main difference
*   with the Mode 7 (where period is controled by the value in OCR2A).
*
*   The frequency of the PWM is controled by the prescaler: A prescaler of 32, will give a timer clock of
*										16MHz / 32 = 500KHz
*   This clock timer will then feed a counter of 8 bits (256), so a period of 500Khz/256 = 1.95KHz is expected
*/



#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
       DDRD |=   _BV(DDD3);  //output pin for OCR2B, this is Arduino pin number
       DDRB |=   _BV(DDD3);  //output pin for OCR2A, this is Arduino pin number


	     // In the next line of code, we:
	     // 1. Set the compare output mode to clear OC2A and OC2B on compare match.
	     //    To achieve this, we set bits COM2A1 and COM2B1 to high.
	     // 2. Set the waveform generation mode to fast PWM (mode 3 in datasheet).
	     //    To achieve this, we set bits WGM21 and WGM20 to high.
	     TCCR2A =  _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);


	     // In the next line of code, we:
	     //  Set the prescaler divisor to 1, so that our counter will be fed with
	     //    the clock's full frequency (16MHz). To achieve this, we set CS20 to
	     //    high (and keep CS21 and CS22 to low by not setting them).
	     TCCR2B =  _BV(CS21) | _BV(CS20); //_BV(WGM22) |


	     // OCR2A control the duty cycle of channel A
	     // A very large duty will appear on pin PB3 here
	     OCR2A = 250;
	     // OCR2B control the duty cycle of channel B
  		 // Only spike (small duty cycle) will appear on PD3
  		 OCR2B = 12;


  // Nothing really happens in the main loop
  while (1);

}
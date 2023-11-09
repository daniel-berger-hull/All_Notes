/*
*   This code is using a CTC Timer, to toggle the pin 6 (PD6 - OCA0).
*   The timer clock is set to 250KHz (16MHz/64 prescaler), and the period is contoled by
*   the value of the Output Compare register  OCA0
*
*/

#include <avr/io.h>


int main(void)
{
	//Place TOP timer value to Output compare register
	OCR0A=225;

	//Set CTC mode
	//and make toggle PD6/OC0A pin on compare match
	TCCR0A |=(1<<COM0A0)| // Toggle OC0A on Compare Match
			 (1<<WGM01);  // CTC - Mode 2

	TCCR0B =  _BV(CS01) | _BV(CS00); // Timer clock of 250KHz

	DDRD |=  _BV(PD6);  //set PD6 and PD2 as output


  // Nothing really happens in the main loop
  while (1);


}
/*
*  GPIO Output Demo
*
*  1) Set the direction of the pin you work on
*       DDRn is a the register to access
*       Each bit of the register represents one pin
*	        Bit 0 (binary value 1)   --> Pin 0, 
*		    Bit 7 (binary value 128) --> Pin 7
*      The bit values represents if pin is output or input
*            0 means Pin Reading
*            1 means Pin Output
*      
*    Example:   
*      (Note: Using binary operator OR, so that other bit on the register are not impacted)
*			DDRB |=  (1<<DDB5);  //Setting Pint #5 of port B as output
*			DDRC |= ~(1<<DDB2);  //Setting Pint #2 of port C as input. 
*                                 //Logic inversion of bit 2 used here
* 
*   2) Output the value on the pin
*      Example:
*      	   PINB |= (1 << PINB5);
*
*
*
*/ 

 #define F_CPU 16000000UL  // 16 MHz

#include <avr/io.h>
#include <util/delay.h>



int main()
{
    //Setting the Data Direction Register for the bit #5
	DDRB |= (1<<DDB5);

	while(1)
	{
	   //
	   PINB |= (1 << PINB5);
	  _delay_ms(500);
	   
	   //Full Port Write example
	   //Write the value 0x20 (32 decimal) to set the bit #5
	   PORTB = 0x20;
	  _delay_ms(1000);
	}
}






// This program demonstrates the use of the ADC with interupt (ADC_vect after conversion completed)
// controler the timer 0. The sampling is done at 20 KHz (Frequency set for Timer 0)
// And to be controlled by a timer, the ADC has to be set to auto-triggering, and also set the trigger
// source to the Timer 0 On compare Match channel A
// that conversion is happening (1 toggle = 100 000 conversions)


#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>


volatile uint16_t valueReading = 0;

void initADC(void)
{

    ADMUX |= (1<<REFS0);	 // Select Vref=AVcc
    						 // ADLAR is kept a 0, so read values are right aligned
    						 // MUX bit are ketp to 0, so the ADC0 is the channel selected

    ADCSRA |=   (1<<ADEN)  | 					     // Enable ADC peripheral
    			(1<<ADATE) | 					     // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
    			(1<<ADIE)  | 					     // ADC Interrupt activated
    		    (1<<ADPS2)|(1<<ADPS0);


    ADCSRB |= (1<<ADTS1)|(1<<ADTS0);    // ADC trigger source - Timer0 compare match A

    ADCSRA |= (1<<ADSC);		// Start Conversion
}

void initGPIO(void)
{
	DDRB |=    _BV(DDB5);  //LED pin in on port B5
}

void InitTimer0(void)
{

	OCR0A=99;		//Place TOP timer value to Output compare register

	TCCR0A |=(1<<COM0A0)|  //and make toggle PD6/OC0A pin on compare match
		     (1<<WGM01);    //Set CTC mode

	TCCR0B |= (1<<CS01);     // Prescaler of 8, frequency of timer0 is 2MHz  (16MHz/8)
}

/*ADC Conversion Complete Interrupt Service Routine (ISR)*/
ISR(ADC_vect)
{
	 //clear timer compare match flag
    TIFR0=(1<<OCF0A);

	valueReading = ADC;
  	// A reading of the 2 registers can also be used, instead of using register ADC
  	// valueReading = ADCL | (ADCH << 8);

	//This pin signal is only to demonstrate the
	//frequency of the ADC conversion (with a logic analyser)
	PORTB |= 0x20;
	_delay_us (1);
	PORTB = 0;

	// If free-running mode is enabled, this next part is tot needed
	//ADCSRA |= 1<<ADSC;		// Restart Conversion
}






int main()
{

	initGPIO();
	InitTimer0();
	initADC();

	sei();				  // Enable global interrupts

	while(1);

}



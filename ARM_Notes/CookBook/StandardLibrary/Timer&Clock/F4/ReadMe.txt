
TIM3_OutputCR_Interrupt_Toggle.c
---------------------------------

This demo toggle the value on port C pins (from 6 to 9), using alternate function
  and the Output Compare mechanism.


BlackBoard_USART_OnePulse.c
---------------------------

[This code been coded and tested to run on a STM32F4 Black Board, not the green one]
This is a demo on the One Pulse Mode, ran on the Timer 4, which output multiple pulse (it can sent just one if wanted) to
the PB6 and PB7 pins. It is also including an USART display and command processor, so the application respond to 
command coming from a Serial Console on a PC.
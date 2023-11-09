RTC (Real Time Clock):
======================

  Simple lib that works good: It can set the Time and Date of the Microconroller, and read the time
  simply. Also, an Interup Handle of the Alarm is present (activate a Green LED for demo)
  
  
 Timer:
========
  
  
  Timer3_GPIO_Interupt
      This is a demo of the Output Compare on 4 differents channel, and 4 LEDS are driven by the timer. The
      Timer Interrput is invoked as soon as one of the 4 channels reaches its output compare set (it maximum
      in other words).
      
   
  Timer3_GPIO_AlternateFunction
      This is a direct way to change the value of the Pin (Here, the PE3). The pin is driven by the Alternate function
      of the Timer #3, channel #2 (see the Alternate Function tab in the STM32_Datasheet.pdf document, page 50).
      
      
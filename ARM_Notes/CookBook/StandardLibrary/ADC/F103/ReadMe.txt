* ADC_Polling.c
  This is the simplest possible way to read an anlog value, but regular polling of the Data Register 
  of the ADC we targetting.
  
* ADC_Tim3_Trigger.c
  This example is based on the use of a timer (Timer3) to drive the rate of the reading, instead of
  polling in a loop like the exmaple ADC_Polling.c. The reading is then done into an interrupt handler
  assigned to the ADC (instead of inside a main loop).

* ADC_DMA.c
   Here, the DMA peripheral is used to transfer automatically the value read by the ADC into a variable
   of the demo program. There is no explicit call to the reading method done by the demo. Also, the ADC
  conversion is contious, like it was the case with the polling example (ADC_Polling.c). This is necessary
  if we want to have converted values to transfer to the program (using the DMA).




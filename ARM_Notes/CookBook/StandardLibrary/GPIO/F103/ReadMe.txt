DigitalRead.c
==============

Show how to read the value of a pin, based on the function GPIO_ReadInputDataBit of the stm32f10x_gpio.h librarie.
That function returns an 8 bits value:

   1 --> Pin has a 3.3V applied to it (a binary 1)
   0 --> Pin is grouned (0 V applied, so a binary 0)


DigitalWrite.c
==============

Show the opposite, how to write a value of a pin, based on the function GPIO_WriteBit of the stm32f10x_gpio.h librarie.
User the constants Bit_SET (value 1) & Bit_RESET (value 0) as parameter of the Write Bit function.


BlinkLed.c
===========
  
Special version of DigitalWrite.c where we flash the User Led on the STM32F103 board.


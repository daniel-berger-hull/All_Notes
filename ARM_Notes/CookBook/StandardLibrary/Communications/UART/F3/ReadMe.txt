usart
=======

  Working lib for the F3, using either USART1/USART2/USART3. the confirmation of the USART to
  use is done via a define statement.
  
  See header for details on the pins to use.
  Tested to work at speed up to 115200 Bauds on all the USART, and using the USB/TTL and FTDI232RL
  
usart_command
==============

  A simple user command processor, using the lib files usart.h/usart.c
  
  This is a starting point for projects that requires control from the PC with
  commands entered on the keyboard.
  

RX_Commands
===========
Deprecated and replaced by usart_command
 Operationnal console command system, receiving commands from a Serial Console, and displaying activity (Reception of char or commands)
 on the Leds
 
 Few demo commands, like 'stop' 'run' and few other been coded.
 
 the demo used the USART1, port PA9 and PA10 of the F3 board

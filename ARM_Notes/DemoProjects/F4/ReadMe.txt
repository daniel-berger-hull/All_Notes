BlackBoard_USART_OnePulse.c
-----------------------------
 
 This is a demo of the One Pulse Mode on Timer 4. It send output to 2 pins (PB6 & PB7), which a driven by the Timer
 Alternate function. The program will alternate between few milliSecond of output of timer followed by no output.
 
 The control is fully done by command from PC, using USART peripheral (see
 USE_USARTx define in usart.c). It accepts few user commands, but type 'run' in a client console
 (i.a Putty) to output on pin PB6 & PB7
 
 Finally, A flashing led method is written to have a physical display visible on the board
 (to show activity with want)
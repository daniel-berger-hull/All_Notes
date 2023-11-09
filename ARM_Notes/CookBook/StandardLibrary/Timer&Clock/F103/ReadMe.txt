
delay_DecreaseCounter.c:
-------------------------

A  brute counter decrease in a loop, with no real Timer hardware used.
It still query the system time frequency, and set a multiplier factor make an adjustment 
to have a better precision...


delay_SysTick.c (.h)
----------------------

Another simple tick counting, that use the main system timer. That timer is handled by
the system handler process SysTick_Handler().


TIM2_PollingValue.c
---------------------

Show the use a simple polling on the value on the timer 2, based on the method TIM_GetCounter


TIM2_PollingFlag.c
-------------------

Very similar to TIM2_PollingValue.c, but here the update interrupt flag is activated, and the main loop queries on the state of 
the flag and turn on/Turn off the User Led. The function TIM_GetITStatus is used to get the value of the flag we want.
Also, the activation of a specific interrupt flag is done with method  TIM_ITConfig(Timer_Number, Flag_To_Activatge, ENABLE);

TIM3_OnePulse.c
-------------------

Use the One Pulse mode, available in the General and Advanced Timer peripherals. The output of the time is sent as output
on a PIN (PB0).

TIM2_3_One_MultiPulse.c
------------------------

This is a one pulse demo, like TIM3_OnePulse.c,  but that use both Timer2 and Timer3. The timers drives
the following pins:
   Timer 2
	PA2 (TIM2_CH3), PA3 (TIM2_CH4) 
   Timer 3
	PB0 (TIM3_CH3), PB1 (TIM3_CH4) 

Check in the F103 Data sheet if you want to use other channels (Channel 1 or Channel 2) for those times
Also, the Timer 2 shows the Multi Pulse, which is a special case of One Pulse Design (bit OPM of TIMx_CR1 )


TIM2_Triger_TIM3OnePulse.c
--------------------------

A one pulse application, but driven (started) by another timer. The master timer (timer 2) is using a 
simple Update Interrupt after it reached it maximum and activate the Timer 3.



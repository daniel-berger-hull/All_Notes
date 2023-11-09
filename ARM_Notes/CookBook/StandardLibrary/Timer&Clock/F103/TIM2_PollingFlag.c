/*******************************************************************************
 * @title        TIM2_PollingFlag.c
 *
 * @Description  Show how to use the Timer 2 and monitor the update interrupt flag, which is set once the counter
 *               reaches:
 *                  For counter up   --> it maximum value (period)
 *                  For counter down --> the value 0
 *
 * 			   The method to query the flag value with the method current value of a timer is:
  	  	  	  	  	  	  	  	  *** TIM_GetITStatus(Timer_Number, FlagToLookFor) ***
 * 				  And here, the interrupt flag to look for is TIM_IT_Update
 *
 * 				Very Similar code to the  TIM2_PollingValue.c, whom check the value of the counter itself, instead
 * 				of the interupt flag of the counter.
 *
 * 				Also the same code should work on Timer 2,Timer 3,Timer 4 and Timer 5
 *
 *   Steps:
 *   1) Activate the clock on the timer to use (here Timer 2)
 *      The function *** RCC_APB2PeriphClockCmd ***    is used
 *      Note: The Timer 2 to 7 are all on the APB1 Bus, which runs at 36 MHz
 *
 *    2) Configure the Timer using the TIM_TimeBaseInitTypeDef
 *       You have to set:
 *       i)  Counting direction:
 *            Counting up          --> Value TIM_CounterMode_Up
 *            Counting down        --> Value TIM_CounterMode_Down
 *            Counting up and down --> Check the various values in  stm32f10x_tim.h
 *       ii)  The Prescaler, or the number of System Ticks to waist between each
 *           ticks happening on the Timer itself.
 *           For instance, the Bus 1 runs at 36Mhz, to 36 millions ticks should go on the timer.
 *           But if we set a prescaler of 1000, then the timer will have only 36000 ticks, instead of
 *           36 millions...
 *       ii) The period, or the number of ticks the timer will count up or down before a reset...
 *           For example, a period of 500 on a counter going up will have values from 0 to 500
 *       iii) The Pin Clock Speed --> 50 Mhz is used,     value GPIO_Speed_50MHz
 *
 *    3) Start the timer with the command
 *                 ***  TIM_Cmd(Timer_Number, ENABLE);

 *    4) Activate the interrupt flags update so that we can poll its value in the main loop
 *       the command to use is:
 * 				***   TIM_ITConfig(Timer_Number, Flag_To_activate, ENABLE) ***
 *    In this example, the call will be:
 *    			--> TIM_ITConfig(Timer_Number, TIM_IT_Update, ENABLE);
 *   Note: Make sure to clear the state of the interrupt flag when we use them in a program
 *         The command is
 *                *** TIM_ClearITPendingBit(Timer_Number, Flag_To_clear) ***
 *
  ********************************************************************************/

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>



void InitializeLEDs()
{
	GPIO_InitTypeDef gpioStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpioStructure.GPIO_Pin =  GPIO_Pin_13;
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
     gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpioStructure);
}

void InitializeTimer()
{
	TIM_TimeBaseInitTypeDef timerInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //Will divide the MCU clock speed by 36000, Timer 2 is on APB1 at 36 Mhz (while APB2 is at 72 Mhz)
    //So:   36000000 / 36000 =  1000 Ticks/Second
    timerInitStructure.TIM_Prescaler = 36000;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //Will request a period of 1 sec, or 1000 ticks on the counter
    timerInitStructure.TIM_Period = 1000;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
    //Make sure to activate the interrupt flag when the timer reach its maximum and its value reset to 0
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}


int main(void)
{
	InitializeLEDs();
	InitializeTimer();

	//We turn on or off  the User Led once every second, Since we have 1000 ticks by second (because of the selected prescaler of 36000),
	//the timer 2, we will reach its maximum value (period is 1000) after a second
	//we will change the state of the led on the interrupt flag is turned on...
	while(1)
    {
    	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    	{
    	     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    	     //This code is only a bit toggle, and is not available on the STM32F103, but is there
    	     //on other STM32 MCU. If you find such method on day, replace this code...
    	     uint8_t state =  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
    	     if (state == 1)
    	    	 GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    	     else
    	    	 GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    	}
    }
}

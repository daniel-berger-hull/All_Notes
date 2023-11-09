/**
  *****************************************************************************
  * @title        TIM2_PollingValue.c
  * @Description  Show how to use the Timer 2 and monitor its value using a polling loop.
  * 			  The method to query the current value of a timer is:
  	  	  	  	  	  	  	  	  *** TIM_GetCounter(Timer_Number) ***
 * 				  This represent the simplest possible way to use a timer.

 * 				  Similar code will work on all General Timer of the STM32F103, so
 * 				  Timer 2,Timer 3,Timer 4 and Timer 5
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
  ********************************************************************************/

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>



void InitializeLEDs()
{
	GPIO_InitTypeDef gpioStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpioStructure.GPIO_Pin =  GPIO_Pin_13;			//The LED pin on the STM32F103C8T6 board is PC13
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
}


int main(void)
{
	InitializeLEDs();
	InitializeTimer();

	//We flash the User Led once by second, to 500 mSec turned on and 500 mSec turned off.
	//Since we have 1000 ticks by second on the timer 2 (because of the selected prescaler of 36000),
	//we turn on for The 500 first ticks and turn of once the timer goes over the value of 500.
    while(1)
    {
    	 int timerValue = TIM_GetCounter(TIM2);
    	 if (timerValue > 500)
    		 GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    	 else
    	     GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    }
}

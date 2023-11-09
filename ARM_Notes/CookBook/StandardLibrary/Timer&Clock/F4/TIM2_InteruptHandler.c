/*******************************************************************************
 * @title        TIM2_InteruptHandler.c
 *
 * @Description  Show how to use the Timer 2 and set and interrupt handler to be called at fixed interval.
 * 				 The handler method is called when the TIM_IT_Update is set, which itself happen
 * 				    For counter up   --> it maximum value (period)
 *                  For counter down --> the value 0
 *
 * 			   The handler to query the flag value with the method current value of a timer is:
  	  	  	  	  	  	  	  	  *** TIM_GetITStatus(Timer_Number, FlagToLookFor) ***
 * 				  And the flag should be SET (otherwise, there is big problem that did happen)
 *
 * 				Also the same code should work on Timer 2,Timer 3,Timer 4 and Timer 5
 *
 *   Steps:
 *   1) Activate the clock on the timer to use (here Timer 2)
 *      The function *** RCC_APB1PeriphClockCmd ***    is used
 *      Note: The Timer 2 to 7 are all on the APB1 Bus, which runs at 42 MHz on an STM32F4
 *
 *    2) Configure the Timer using the TIM_TimeBaseInitTypeDef
 	 	 	 	 	 	 	 	  *
 *       You have to set:
 *       i)  Counting direction:
 *            Counting up          --> Value TIM_CounterMode_Up
 *            Counting down        --> Value TIM_CounterMode_Down
 *            Counting up and down --> Check the various values in  stm32f10x_tim.h
 *       ii)  The Prescaler, or the number of System Ticks to waist between each
 *           ticks happening on the Timer itself.
 *           For instance, the Bus 1 runs at 42Mhz, then 42 millions ticks should go on the timer.
 *           But if we set a prescaler of 1000, then the timer will have only 42 000 ticks, instead of
 *           42 millions...
 *       ii) The period, or the number of ticks the timer will count up or down before a reset...
 *           For example, a period of 500 on a counter going up will have values from 0 to 500
 *       iii) The Pin Clock Speed --> 100 Mhz is used,     value GPIO_Speed_100MHz
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

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"


//Will flash the Green Led on the board, which is the pin 12 of the port D
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

}


/*
 *  The timer 2 will have a prescaler set so that we reduce its frequency from the default 42MHz
 *  (Default speed on Bus APB1) down to 10Khz.
 *       -->   42000000/4200 = 10000
 *   And we will let the timer count up till 1000, so the period will be 1000.
 */
void TIM2_Config(){

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period =1000;
	TIM_TimeBaseStructure.TIM_Prescaler = 4200;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}

/*
 *   Interrupt Method every time the Timer 2 is run above its period (overflow).
 *   The Flag TIM_IT_Update is then set, meaning that the Value of the timer will be updated
 *   (to the value 0 in the case of an up counter timer)
 *
 */
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	}
}

void NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

int main(void){

	SystemInit();
	GPIO_Config();
	TIM2_Config();
	NVIC_Config();

	while (1){
	}
}

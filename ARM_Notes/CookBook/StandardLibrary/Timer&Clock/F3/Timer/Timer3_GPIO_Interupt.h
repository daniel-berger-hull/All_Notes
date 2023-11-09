/******************************************************************************
  * @file    Timer3_GPIO_Interupt.h
  * @author  Daniel Berger
  * @version V1.0.1
  * @date    01-April-2017
  * @brief   See the file Timer3_GPIO_Interupt.c for details on the code
  *
  *******************************************************************************/



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER3_GPIO_INTERRUPT_H
#define __TIMER3_GPIO_INTERRUPT_H


#include "main.h"

void initTimer3(void);
void activateLED(int index);
void TIM3_IRQHandler(void);


#endif /* __TIMER3_GPIO_INTERRUPT_H */

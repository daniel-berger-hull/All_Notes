/*******************************************************************************
 * @title        delay_SysTick.h
 *
 * @Description  Show how to use the ARM Cortex System Tick mecanism
 *				  See delay_SysTick.c for the details of the code example
 *
 ********************************************************************************/


#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"

void delay(uint32_t time, uint32_t load);
void delay_us(uint32_t time);
void delay_ms(uint32_t time);

#endif	/* DELAY_H */

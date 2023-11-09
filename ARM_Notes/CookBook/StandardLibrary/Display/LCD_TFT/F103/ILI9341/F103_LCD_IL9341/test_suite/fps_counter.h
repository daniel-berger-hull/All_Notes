#ifndef TEST1_FPS_COUNTER_H
#define TEST1_FPS_COUNTER_H

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include "tinystd/itoa.h"


void          FPS_start();
void          FPS_frame();
unsigned char *FPS_getText();

#endif //TEST1_FPS_COUNTER_H

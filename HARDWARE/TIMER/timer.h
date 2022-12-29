#ifndef __TIMER_H
#define __TIMER_H
#include "system.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void Timerx_Init(u16 arr,u16 psc); 


void TIME2_Init(void);
void TIM2_Configuration(void);
void TIM2_NVIC_Configuration(void);


#endif

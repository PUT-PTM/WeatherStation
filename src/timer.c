/*
File: gpio.c
Description: Timer module
Author: Adam Orcholski, tath@o2.pl, www.tath.eu
Log (day/month/year):
- (07.04.2013) Initial
- (19.07.2015) Refactored
Note:
 - refer to official documentation to understand what is happening here
*/

#include <stm32f10x.h>
#include "timer.h"

/* Public function definitions */
void TIM2_Init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
 
    TIM2->CR1 = 0;
    TIM2->ARR = 0xffff;
    TIM2->PSC = 71;
    TIM2->EGR = TIM_EGR_UG;
}

void TIM2_Deinit(void)
{
    RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;
}

void TIM2_Enable(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_Disable(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

void TIM2_DelayUs(const uint16_t delay)
{
    TIM2->CNT = 0;
    while((uint16_t)(TIM2->CNT) <= delay);
}

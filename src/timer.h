/*
File: timer.h
Description: timer2 module configured for microsecond delay
Author: Adam Orcholski, tath@o2.pl, www.tath.eu
Log (day/month/year):
- (07.04.2013) Initial
- (19.07.2015) Refactored
*/
#ifndef _tim_def_
#define _tim_def_

#include <stdint.h>

/* Public function declarations */
void TIM2_Init(void);
void TIM2_Deinit(void);
void TIM2_Enable(void);
void TIM2_Disable(void);
void TIM2_DelayUs(const uint16_t delay);
#endif

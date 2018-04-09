/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

void delay(int time)
{
    int i;
    for (i = 0; i < time * 4000; i++) {}
}

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpio;  // obiekt gpio bêd¹cy konfiguracj¹ portów GPIO

	GPIO_StructInit(&gpio);  // domyœlna konfiguracja
	gpio.GPIO_Pin = GPIO_Pin_5;  // konfigurujemy pin 5
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;  // jako wyjœcie
	GPIO_Init(GPIOA, &gpio);  // inicjalizacja modu³u GPIOA

	while (1) {
		GPIO_SetBits(GPIOA, GPIO_Pin_5); // zapalenie diody
		delay(500); // delay
		GPIO_ResetBits(GPIOA, GPIO_Pin_5); // zgaszenie diody
		delay(2000);
	}
}

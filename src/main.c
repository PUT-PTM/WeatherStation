#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"

int main(void)
{
	GPIO_InitTypeDef gpio;
	SPI_InitTypeDef spi;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef  channel;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; // SCK, MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_6; // MISO
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = LCD_DC|LCD_CE|LCD_RST;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio);
	GPIO_SetBits(GPIOC, LCD_CE|LCD_RST);

	gpio.GPIO_Pin = GPIO_Pin_6;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpio);

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	TIM_TimeBaseStructInit(&tim);
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Prescaler = 6400 - 1;
	tim.TIM_Period = 100 - 1;
	TIM_TimeBaseInit(TIM4, &tim);

	TIM_OCStructInit(&channel);
	channel.TIM_OCMode = TIM_OCMode_PWM1;
	channel.TIM_OutputState = TIM_OutputState_Enable;
	channel.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &channel);
	TIM_Cmd(TIM4, ENABLE);

	SysTick_Config(SystemCoreClock / 1000);

	lcd_setup();

	lcd_draw_text(1, -44, "Stacja");
	lcd_draw_text(2, -33, "pogodowa :)");
	lcd_draw_text(3, -22, "Jakub Lorc");
	lcd_draw_text(4, -10, "Wojciech");
	lcd_draw_text(5, -9, "Krasicki");
	lcd_copy();
	delay_ms(6000);

	lcd_clear();
	lcd_draw_text(1, -44, "Temperatura: ");
	lcd_draw_text(3, -44, "Cisnienie: ");
	lcd_copy();
	TIM_SetCompare1(TIM4, fabs((100)));

	while (1) {}
}

#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "gpio.h"
#include "SHT3x.h"

extern uint8_t 	SHT_Data8[6];
double tempValue;
double humValue;
char buffer[10];

void send_char(char c)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}

int __io_putchar(int c)
{
	if (c=='\n')
		send_char('\r');
	send_char(c);
	return c;
}

void computeTemperatureAndHumidity() {
	static uint16_t temp16;
	static uint16_t hum16;

	temp16 = SHT_Data8[1] | (SHT_Data8[0]<<8);
	hum16 = SHT_Data8[4] | (SHT_Data8[3]<<8);

	tempValue = -45.0 + 175.0*((double)temp16/65535.0);
	humValue = 100.0*((double)hum16/65535.0);
}

int main(void)
{
	GPIO_InitTypeDef gpio;
	I2C_InitTypeDef i2c;
	USART_InitTypeDef uart;
	SPI_InitTypeDef spi;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio);

	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio);

	USART_StructInit(&uart);
	uart.USART_BaudRate = 115200;
	USART_Init(USART2, &uart);
	USART_Cmd(USART2, ENABLE);

	gpio.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; // SCL, SDA
	gpio.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
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

	gpio.GPIO_Pin = GPIO_Pin_8;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio);

	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);

	I2C_StructInit(&i2c);
	i2c.I2C_Mode = I2C_Mode_I2C;
	i2c.I2C_ClockSpeed = 100000;
	i2c.I2C_Ack = I2C_Ack_Enable;
	i2c.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	i2c.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C_Init(I2C1, &i2c);
	I2C_Cmd(I2C1, ENABLE);

	SysTick_Config(SystemCoreClock / 1000);

	lcd_setup();

	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	lcd_draw_text(1, -44, "Weather");
	lcd_draw_text(2, -22, "Station :)");
	lcd_draw_text(3, -22, "Jakub Lorc");
	lcd_draw_text(4, -14, "Wojciech");
	lcd_draw_text(5, -14, "Krasicki");
	lcd_copy();
	delay_ms(4000);

	while (1) {
		SHT_StartMeasure();
		delay_ms(1000);
		SHT_Data_Read();
		computeTemperatureAndHumidity();

		lcd_clear();
		sprintf(buffer,"Temp: %.2f%cC", tempValue, 128);
		lcd_draw_text(1, -44, buffer);
		sprintf(buffer,"Hum: %.2f%%", humValue);
		lcd_draw_text(4, -44, buffer);
		lcd_copy();
	}
}

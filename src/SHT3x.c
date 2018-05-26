// MPU-6050 c file

#include "SHT3x.h"

#include "stm32f10x.h"

uint8_t 	SHT_Data8[6];

void SHT_StartMeasure()
{
	volatile uint8_t txtmp = 0;

	I2C1->CR1 |= I2C_CR1_START;					//START
  	while(!(I2C1->SR1 & I2C_SR1_SB));			

	I2C1->DR = SHT_Address_W;					//MPU
	while(!(I2C1->SR1 & I2C_SR1_ADDR));

	txtmp = I2C1->SR2;							//Read SR2 = Zero ADDR

	I2C1->DR = 0x24;								//MPU Address to save
	while(!(I2C1->SR1 & I2C_SR1_TXE));

	I2C1->DR = 0x16;								//MPU Data to save
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	while(!(I2C1->SR1 & I2C_SR1_BTF));

	I2C1->CR1 |= I2C_CR1_STOP;
}

void SHT_Data_Read(void)
{
 	uint8_t i = 0;

 	I2C1->CR1 |= I2C_CR1_START;					//START
  	while(!(I2C1->SR1 & I2C_SR1_SB));

	I2C1->DR = SHT_Address_R;					//MPU
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	i = I2C1->SR2;								//Read SR2 = Zero ADDR

	for(i=0; i < 3; i++)
	{
		while(!(I2C1->SR1 & I2C_SR1_RXNE));
		SHT_Data8[i] = I2C1->DR;
	}

	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	while(!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 &= ~I2C_CR1_ACK;					//ACK OFF

	SHT_Data8[3] = I2C1->DR;

	I2C1->CR1 |= I2C_CR1_STOP;					//I2C STOP

	SHT_Data8[4] = I2C1->DR;

	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	SHT_Data8[5] = I2C1->DR;

	I2C1->CR1 |= I2C_CR1_ACK;					//ACK ON
	I2C1->CR1 |= I2C_CR1_STOP;
}

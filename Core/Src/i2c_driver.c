#include "i2c_driver.h"

#define AF4 GPIO_AFRL_AFSEL6_2
#define I2C_CCR_VALUE 0x50UL

void I2C_init(I2C_TypeDef* i2cx)
{
	//SCL (PB6) SDA (PB7)
	// clock enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // PORTB clock enable
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C1 clock enable

	// setting SCL, SDA to alternate function mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7); // clear
	GPIOB->MODER |= GPIO_MODER_MODER6_1; //  (10 is af mode)
	GPIOB->MODER |= GPIO_MODER_MODER7_1; //  (10 is af mode)

	// setting SCL, SDA to output open drain
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7); // clear
	GPIOB->OTYPER |= GPIO_OTYPER_OT6;
	GPIOB->OTYPER |= GPIO_OTYPER_OT7;

	// setting SCL, SDA to high speed outputs
	GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7); // clear
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED7;

	// setting SCL, SDA to AF4 ( I2C1)
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7); // clear
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_2; // 100 is af4
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;

	// I2C register settings
	i2cx->CR2 |= I2C_CR2_FREQ_4; // 16mHz
	i2cx->CCR |= (I2C_CCR_VALUE << I2C_CCR_CCR_Pos); // 80U (5us / 62.5ns)
	i2cx->TRISE |= ((I2C_CR2_FREQ_4 | I2C_CR2_FREQ_0) << I2C_TRISE_TRISE_Pos); // TRISE = CR2 freq + 1

	//enable I2C
	i2cx->CR1 |= I2C_CR1_PE;
}

void I2C_start(I2C_TypeDef* i2cx)
{
	i2cx->CR1 |= I2C_CR1_START;

	while(!(i2cx->SR1 & (1 << 0)));
}

void I2C_writeData(I2C_TypeDef* i2cx, uint8_t data)
{
	i2cx->DR = data;

	while(!(i2cx->SR1 & (1 << 2)));
}

void I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr)
{
	i2cx->DR = addr;

	while(!(i2cx->SR1 & (1 << 1)));

	uint8_t temp = i2cx->SR1 | i2cx->SR2; // read SR2 and SR1 (clear ADDR bit)
	(void)temp;
}

void I2C_stop(I2C_TypeDef* i2cx)
{
	i2cx->CR1 |= I2C_CR1_STOP;
}

void I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size)
{
	slaveAddress = slaveAddress << 1;

	I2C_start(i2cx);
	I2C_sendAddr(i2cx, slaveAddress);
	I2C_writeData(i2cx, startReg);

	I2C_start(i2cx);

	i2cx->DR = slaveAddress | 1;
	while(!(i2cx->SR1 & (1 << 1)));
	i2cx->CR1 |= (1 << 10);
	uint8_t temp = i2cx->SR1 | i2cx->SR2;
	(void)temp;

	for (size_t i = 0; i < size; i++) {
		if (i == size - 1) {
			i2cx->CR1 &= ~(I2C_CR1_ACK); //NACK
			I2C_stop(i2cx);
		}
		while (!(i2cx->SR1 & (1 << 6)));
		buffer[i] = i2cx->DR;
	}
}

#include "i2c_driver.h"

#define AF4 GPIO_AFRL_AFSEL6_2
#define I2C_CCR_VALUE 0x50UL
#define I2C_TIMEOUT_VALUE 50000

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

I2C_Status I2C_start(I2C_TypeDef* i2cx)
{
	uint32_t timeout = I2C_TIMEOUT_VALUE;
	i2cx->CR1 |= I2C_CR1_START;

	while(!(i2cx->SR1 & I2C_SR1_SB)) {
		if (--timeout == 0) return I2C_TIMEOUT;
	}
	return I2C_OK;
}

I2C_Status I2C_writeData(I2C_TypeDef* i2cx, uint8_t data)
{
	uint32_t timeout = I2C_TIMEOUT_VALUE;
	i2cx->DR = data;

	while(!(i2cx->SR1 & I2C_SR1_TXE)) {
		if (--timeout == 0) return I2C_TIMEOUT;
	}

	return I2C_OK;
}

I2C_Status I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr)
{
	uint32_t timeout = I2C_TIMEOUT_VALUE;
	i2cx->DR = addr;

	while(!(i2cx->SR1 & I2C_SR1_ADDR)){
		if(--timeout == 0) return I2C_TIMEOUT;
	}

	uint8_t temp = i2cx->SR1 | i2cx->SR2; // read SR2 and SR1 (clear ADDR bit)
	(void)temp;

	return I2C_OK;
}

void I2C_stop(I2C_TypeDef* i2cx)
{
	i2cx->CR1 |= I2C_CR1_STOP;
}

I2C_Status I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size)
{
	uint32_t timeout = I2C_TIMEOUT_VALUE;

	//init
	if(I2C_start(i2cx) != I2C_OK) return I2C_TIMEOUT;
	if(I2C_sendAddr(i2cx, (slaveAddress << 1)) != I2C_OK) return I2C_TIMEOUT;
	if (I2C_writeData(i2cx, startReg) != I2C_OK) return I2C_TIMEOUT;

	I2C_start(i2cx);

	i2cx->DR = (slaveAddress << 1) | 1;
	while(!(i2cx->SR1 & I2C_SR1_ADDR)) {
		if (--timeout == 0) return I2C_TIMEOUT;
	};

	i2cx->CR1 |= I2C_CR1_ACK;
	uint8_t temp = i2cx->SR1 | i2cx->SR2;
	(void)temp;

	for (size_t i = 0; i < size; i++) {
		if (i == size - 1) {
			i2cx->CR1 &= ~(I2C_CR1_ACK); //NACK
			I2C_stop(i2cx);
		}

		timeout = I2C_TIMEOUT_VALUE;
		while (!(i2cx->SR1 & I2C_SR1_RXNE)) {
			if(--timeout == 0) return I2C_TIMEOUT;
		}

		buffer[i] = i2cx->DR;
	}
}

I2C_Status I2C_burstRead_DMA(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size)
{
    uint32_t timeout = I2C_TIMEOUT_VALUE;

    // init
    if(I2C_start(i2cx) != I2C_OK) return I2C_TIMEOUT;
    if(I2C_sendAddr(i2cx, slaveAddress << 1) != I2C_OK) return I2C_TIMEOUT;
    if(I2C_writeData(i2cx, startReg) != I2C_OK) return I2C_TIMEOUT;

    // prepare dma
    DMA_Config_I2C1_RX(buffer, size);

    // enable i2c dma requests and auto nack
    i2cx->CR2 |= (I2C_CR2_DMAEN | I2C_CR2_LAST);

    // start read phase
    if(I2C_start(i2cx) != I2C_OK) return I2C_TIMEOUT;

    i2cx->DR = (slaveAddress << 1) | 1;

    // wait addr flag
    while(!(i2cx->SR1 & I2C_SR1_ADDR)) {
        if (--timeout == 0) return I2C_TIMEOUT;
    }
    i2cx->CR1 |= I2C_CR1_ACK;

    // clear addr flag
    uint32_t temp = i2cx->SR1 | i2cx->SR2;
    (void)temp;

    return I2C_OK;
}

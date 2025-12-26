#include <i2c_driver.h>

#define AF4 0x4

void I2C_init(I2C_TypeDef* i2cx) {
	//SCL (PB6) SDA (PB7)
	// clock enable
	RCC->AHB1ENR |= (1 << 1); // PORTB clock enable
	RCC->APB1ENR |= (1 << 21); // I2C1 clock enable

	// setting SCL, SDA to alternate function mode
	GPIOB->MODER &= ~(0xF << 12);
	GPIOB->MODER |= (2 << 12); //  (10 is af mode)
	GPIOB->MODER |= (2 << 14); //  (10 is af mode)

	// setting SCL, SDA to output open drain
	GPIOB->OTYPER &= ~(3 << 6);
	GPIOB->OTYPER |= (1 << 6);
	GPIOB->OTYPER |= (1 << 7);


	// setting SCL, SDA to high speed outputs
	GPIOB->OSPEEDR |= (3U << 12);
	GPIOB->OSPEEDR |= (3U << 14);

	// setting SCL, SDA to AF4 ( I2C1)
	GPIOB->AFR[0] &= ~(0xFF << 24);
	GPIOB->AFR[0] |= (AF4 << 24);
	GPIOB->AFR[0] |= (AF4 << 28);

	// I2C register settings
	i2cx->CR2 |= (16U << 0);
	i2cx->CCR |= (0x50 << 0); // 80U (5us / 62.5ns)
	i2cx->TRISE |= (17U << 0); // TRISE = CR2 freq + 1

	//enable I2C
	i2cx->CR1 |= (1U << 0);

}

void I2C_start(I2C_TypeDef* i2cx) {
	i2cx->CR1 |= (1 << 8);

	while(!(i2cx->SR1 & (1 << 0)));
}

void I2C_writeData(I2C_TypeDef* i2cx, uint8_t data) {
	i2cx->DR = data;

	while(!(i2cx->SR1 & (1 << 2)));
}

void I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr) {
	i2cx->DR = addr;

	while(!(i2cx->SR1 & (1 << 1)));

	uint8_t temp = i2cx->SR1 | i2cx->SR2; // read SR2 and SR1 (clear ADDR bit)
	(void)temp;
}

void I2C_stop(I2C_TypeDef* i2cx) {
	i2cx->CR1 |= (1 << 9);
}

void I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size) {
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
			i2cx->CR1 &= ~(1 << 10); //NACK
			I2C_stop(i2cx);
		}
		while (!(i2cx->SR1 & (1 << 6)));
		buffer[i] = i2cx->DR;
	}
}

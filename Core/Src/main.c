#include "stm32f4xx.h"
#define AF4 0x4

void I2C1_init(void) {
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
	I2C1->CR2 |= (16U << 0);
	I2C1->CCR |= (0x50 << 0); // 80U (5us / 62.5ns)
	I2C1->TRISE |= (17U << 0); // TRISE = CR2 freq + 1

	//enable I2C
	I2C1->CR1 |= (1U << 0);

}

void I2C1_start(void) {
	I2C1->CR1 |= (1 << 8);

	while(!(I2C1->SR1 & (1 << 0)));
}

void I2C1_writedata(uint8_t data) {
	I2C1->DR = data;

	while(!(I2C1->SR1 & (1 << 2)));
}

void I2C1_sendaddr(uint8_t addr) {
	I2C1->DR = addr;

	while(!(I2C1->SR1 & (1 << 1)));

	uint8_t temp = I2C1->SR1 | I2C1->SR2; // read SR2 and SR1 (clear ADDR bit)
	(void)temp;
}

void I2C1_stop(void) {
	I2C1->CR1 |= (1 << 9);
}

uint8_t I2C1_read(uint8_t reg) {
	I2C1_start();
	I2C1_sendaddr(0xD0); // MPU-6050 address write
	I2C1_writedata(reg);

	I2C1_start();
	I2C1->DR = 0xD1;
	while(!(I2C1->SR1 & (1 << 1)));
	I2C1->CR1 &= ~(1 << 10);
	uint8_t temp = I2C1->SR1 | I2C1->SR2; // read SR2 and SR1 (clear ADDR bit)
	(void)temp;

	I2C1_stop();
	while(!(I2C1->SR1 & (1 << 6)));

	return I2C1->DR;
}

int16_t read_X_axis() {
	uint8_t reg_high = I2C1_read(0x3B);
	uint8_t reg_low  = I2C1_read(0x3C);

	return (int16_t)((reg_high << 8) | reg_low);
}

int16_t read_Y_axis() {
	uint8_t reg_high = I2C1_read(0x3D);
	uint8_t reg_low  = I2C1_read(0x3E);

	return (int16_t)((reg_high << 8) | reg_low);
}

int16_t read_Z_axis() {
	uint8_t reg_high = I2C1_read(0x3F);
	uint8_t reg_low  = I2C1_read(0x40);

	return (int16_t)((reg_high << 8) | reg_low);
}

volatile int16_t date_cititeX = 0;
volatile int16_t date_cititeY = 0;
volatile int16_t date_cititeZ = 0;

int main(void) {
	I2C1_init();

	uint8_t slave_address = 0xD0;

	I2C1_start();

	I2C1_sendaddr(slave_address);

	I2C1_writedata(0x6B);
	I2C1_writedata(0x00);

	I2C1_stop();

	while(1) {
		date_cititeX = read_X_axis();
		date_cititeY = read_Y_axis();
		date_cititeZ = read_Z_axis();
		for (volatile int i = 0 ; i < 50000; i++);
	}
	return 0;
}

#include "stm32f4xx.h"
#include "i2c_driver.h"

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

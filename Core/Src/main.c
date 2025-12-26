#include "stm32f4xx.h"
#include "mpu6050.h"

MPU_axis data;


int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	MPU_init(I2C1);
	GPIOC->MODER &= ~(3U << 26);
	GPIOC->MODER |= (1U << 26);

	while(1) {
		MPU_readXYZ(I2C1, &data);
		if (data.X > 3000 || data.Y > 3000)
		{
			GPIOC->ODR |= (1 << 13);
		}
		else if (data.X < -3000 || data.Y < -3000)
		{
			GPIOC->ODR |= (1 << 13);
		}
		else
		{
			GPIOC->ODR &= ~(1 << 13);
		}
	}
	return 0;
}

#include <stdlib.h>
#include "stm32f4xx.h"
#include "mpu6050.h"
#include "uart_driver.h"
#include "imu_math.h"
#include "leds.h"

MPU_axis data;


void FPU_enable(void) {
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}

int main(void)
{
	FPU_enable();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	MPU_init(I2C1);
	UART_init();
	GPIOC->MODER &= ~(3U << 26);
	GPIOC->MODER |= (1U << 26);

	while(1)
	{
		char buffer[10];

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


		//
		UART_printXYZ(&data);
		for(volatile int i = 0; i < 2000000; i++);

	}
	return 0;
}

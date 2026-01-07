#include <stdlib.h>
#include "stm32f4xx.h"
#include "mpu6050.h"
#include "uart_driver.h"
#include "imu_math.h"
#include "leds.h"

MPU_axis data;
Angles_t angles;

void FPU_enable(void) {
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}

int main(void)
{
	FPU_enable();
	MPU_init(I2C1);
	UART_init();
	leds_init();
	while(1)
	{

		MPU_readXYZ(I2C1, &data);
		calculateDegrees(&angles, data.X, data.Y, data.Z);
		UART_printXYZ(&data);
		leds_update(angles.roll);
		for(volatile int i = 0; i < 100000; i++);

	}
	return 0;
}

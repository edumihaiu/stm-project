#include <stdlib.h>
#include "stm32f4xx.h"
#include "mpu6050.h"
#include "uart_driver.h"
#include "imu_math.h"
#include "leds.h"
#include "dma_manager.h"

MPU_axis data;
Angles_t angles;

void FPU_enable(void) {
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}

int main(void)
{
	FPU_enable();
	DMA_Manager_init();
	MPU_init(I2C1);
	UART_init();
	leds_init();
	UART_print("Sistem Pornit!\r\n");
	while(1)
	{

		if(MPU_startReadDMA(I2C1) == I2C_OK) {
			while(!DMA_I2C1_IsComplete());
			MPU_parseDataDMA(I2C1, &data);

			calculateDegrees(&angles, data.X, data.Y, data.Z);
			UART_printXYZ(&data);
			leds_update(angles.roll);
			for(volatile int i = 0; i < 100000; i++);
		} else {
			UART_print("eroare i2c!\r\n");
		}
	}
	return 0;
}

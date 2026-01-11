	#include <stdlib.h>
	#include "stm32f4xx.h"
	#include "mpu6050.h"
	#include "uart_driver.h"
	#include "imu_math.h"
	#include "leds.h"
	#include "dma_manager.h"
#include "iwdg_driver.h"

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

		MPU_axis data;
		Angles_t angles;
		Filter filter;

		filter_init(&filter, 10);
		IWDG_init();
		while(1)
		{
			IWDG_refresh();
			if(MPU_startReadDMA(I2C1) == I2C_OK) {
				while(!DMA_I2C1_IsComplete()) {
					__NOP();
				}
				MPU_parseDataDMA(I2C1, &data);
				mouse_logic(&data);
				calculateDegrees(&angles, data.X, data.Y, data.Z);
				float out;
				if(filter_update(&filter, angles.roll, &out))
						leds_update(out);
			} else {
				UART_print("eroare i2c!\r\n");
			}
		}
		return 0;
	}

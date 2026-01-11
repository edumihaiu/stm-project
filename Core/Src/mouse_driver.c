/*
 * mouse_driver.c
 *
 *  Created on: Jan 12, 2026
 *      Author: edu
 */


#include "mouse_driver.h"
#include <string.h>

#define MOUSE_SENSITIVITY 150
#define MOUSE_DEADZONE 2

void mouse_logic(MPU_axis* data_from_mpu)
{
	static char buffer[32];

		// Calcul coordonate
		int mouse_x = -data_from_mpu->Gz / MOUSE_SENSITIVITY;
		int mouse_y = -data_from_mpu->Gx / MOUSE_SENSITIVITY;

		// Deadzone check
		if (abs(mouse_x) < MOUSE_DEADZONE) mouse_x = 0;
		if (abs(mouse_y) < MOUSE_DEADZONE) mouse_y = 0;

		// Trimite doar daca e miscare
		if(!DMA_USART1_IsBusy())
		{
			if (mouse_x != 0 || mouse_y != 0) {
				sprintf(buffer, "%d,%d\r\n", mouse_x, mouse_y);
		    	DMA_Config_USART1_TX(buffer, strlen(buffer));
				leds_update(10.0f); // Feedback vizual
			} else {
				leds_update(0.0f);
			}
		}
}

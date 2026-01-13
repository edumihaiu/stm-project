/*
 * mouse_driver.c
 *
 *  Created on: Jan 12, 2026
 *      Author: edu
 */


#include "mouse_driver.h"
#include <string.h>
#include "imu_math.h"

#define MOUSE_SENSITIVITY 150
#define MOUSE_DEADZONE 2

static Kalman_t filterX;
static Kalman_t filterY;

void mouse_init(void) {
    kalman_init(&filterX);
    kalman_init(&filterY);
}

void mouse_logic(MPU_axis* data_from_mpu)
{
		static char buffer[32];
		float dt = 0.01f;
		float cleanGz = kalman_get_angle(&filterX, 0.0f, (float)data_from_mpu->Gz, dt);
		float cleanGx = kalman_get_angle(&filterY, 0.0f, (float)data_from_mpu->Gx, dt);
		// Calcul coordonate
		int mouse_x = -cleanGz / MOUSE_SENSITIVITY;
		int mouse_y = -cleanGx / MOUSE_SENSITIVITY;

		// Deadzone check
		if (abs(mouse_x) < MOUSE_DEADZONE) mouse_x = 0;
		if (abs(mouse_y) < MOUSE_DEADZONE) mouse_y = 0;

		// Trimite doar daca e miscare
		if(!DMA_USART1_IsBusy())
		{
			if (mouse_x != 0 || mouse_y != 0) {
				sprintf(buffer, "%d,%d\r\n", mouse_x, mouse_y);
		    	DMA_Config_USART1_TX(buffer, strlen(buffer));
			}
		}
}

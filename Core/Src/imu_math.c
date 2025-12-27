/*
 * imu_math.c
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#include "imu_math.h"
#include <math.h>

void calculateDegrees(Angles_t* angles, int16_t X, int16_t Y, int16_t Z)
{
	float ax = (float)X;
	float ay = (float)Y;
	float az = (float)Z;

	angles->roll = atan2f(ay, az) * RAD_TO_DEG;
	angles->pitch = atan2f(-ax, sqrtf(ay*ay + az*az)) * RAD_TO_DEG;
}

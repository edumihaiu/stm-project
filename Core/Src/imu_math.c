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

void filter_init(Filter* filter, uint32_t num_of_samples_needed)
{
	filter->sum = 0;
	filter->counter = 0;
	filter->samples_needed = num_of_samples_needed;
}

uint8_t filter_update(Filter* filter, float new_angle, float* output)
{
	filter->sum += new_angle;
	filter->counter++;

	if (filter->counter == filter->samples_needed)
	{
		*output = filter->sum / filter->counter;
		filter->sum = 0;
		filter->counter = 0;
		return 1;
	}
	return 0;
}

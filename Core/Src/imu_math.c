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


void kalman_init(Kalman_t* filter) {
    filter->Q_angle = 0.001f;
    filter->Q_bias = 0.003f;
    filter->R_measure = 0.03f;

    filter->angle = 0.0f;
    filter->bias = 0.0f;

    filter->P[0][0] = 0.0f;
    filter->P[0][1] = 0.0f;
    filter->P[1][0] = 0.0f;
    filter->P[1][1] = 0.0f;
}

float kalman_get_angle(Kalman_t* filter, float newAngle, float newRate, float dt) {
    // predict
    float rate = newRate - filter->bias;
    filter->angle += dt * rate;

    filter->P[0][0] += dt * (dt * filter->P[1][1] - filter->P[0][1] - filter->P[1][0] + filter->Q_angle);
    filter->P[0][1] -= dt * filter->P[1][1];
    filter->P[1][0] -= dt * filter->P[1][1];
    filter->P[1][1] += filter->Q_bias * dt;

    // update
    float S = filter->P[0][0] + filter->R_measure;
    float K[2]; // gain
    K[0] = filter->P[0][0] / S;
    K[1] = filter->P[1][0] / S;

    float y = newAngle - filter->angle;
    filter->angle += K[0] * y;
    filter->bias += K[1] * y;

    float P00_temp = filter->P[0][0];
    float P01_temp = filter->P[0][1];

    filter->P[0][0] -= K[0] * P00_temp;
    filter->P[0][1] -= K[0] * P01_temp;
    filter->P[1][0] -= K[1] * P00_temp;
    filter->P[1][1] -= K[1] * P01_temp;

    return filter->angle;
}

/*
 * imu_math.h
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#ifndef INC_IMU_MATH_H_
#define INC_IMU_MATH_H_

#include <stdint.h>

#define RAD_TO_DEG 57.2957795f

typedef struct {
	float roll;
	float pitch;
} Angles_t;

typedef struct {
	float sum;
	uint8_t counter;
	uint8_t samples_needed;
} Filter;

void calculateDegrees(Angles_t* angles, int16_t X, int16_t Y, int16_t Z);
void filter_init(Filter* filter, uint32_t num_of_samples_needed);
uint8_t filter_update(Filter* filter, float new_angle, float* output);

#endif /* INC_IMU_MATH_H_ */

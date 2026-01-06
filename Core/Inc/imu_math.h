/*
 * imu_math.h
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#ifndef INC_IMU_MATH_H_
#define INC_IMU_MATH_H_

#include "stm32f4xx.h"


#define RAD_TO_DEG 57.2957795f

typedef struct {
	float roll;
	float pitch;
} Angles_t;

#endif /* INC_IMU_MATH_H_ */

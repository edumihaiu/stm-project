/*
 * mpu6050.h
 *
 *  Created on: Dec 23, 2025
 *      Author: edu
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f4xx.h"

#define MPU_address 0x68
#define MPU_startRegister 0x3B
#define MPU_powerMgmtRegister 0x6B


typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
} MPU_axis;

void MPU_init(I2C_TypeDef* i2cx);
void MPU_readXYZ(I2C_TypeDef* i2cx, MPU_axis* dataStruct);

#endif /* INC_MPU6050_H_ */

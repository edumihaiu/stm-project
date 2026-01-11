/*
 * mpu6050.h
 *
 *  Created on: Dec 23, 2025
 *      Author: edu
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "i2c_driver.h"

typedef struct {
	int16_t X;
	int16_t Y;
	int16_t Z;
	int16_t Gx;
	int16_t Gy;
	int16_t Gz;
} MPU_axis;

void MPU_init(I2C_TypeDef* i2cx);
I2C_Status MPU_parseData(I2C_TypeDef* i2cx, MPU_axis* dataStruct);
I2C_Status MPU_startReadDMA(I2C_TypeDef* i2cx);
I2C_Status MPU_parseDataDMA(I2C_TypeDef* i2cx, MPU_axis* dataStruct);

#endif /* INC_MPU6050_H_ */

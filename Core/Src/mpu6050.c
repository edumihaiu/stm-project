/*
 * mpu6050.c
 *
 *  Created on: Dec 23, 2025
 *      Author: edu
 */


#include "stm32f4xx.h"
#include "i2c_driver.h"
#include "mpu6050.h"


void MPU_init(I2C_TypeDef* i2cx) {
	I2C_init(i2cx);
	I2C_start(i2cx);
	I2C_sendAddr(i2cx, MPU_address << 1);
	I2C_writeData(i2cx, MPU_powerMgmtRegister);
	I2C_writeData(i2cx, 0x00);
	I2C_stop(i2cx);
}

void MPU_readXYZ(I2C_TypeDef* i2cx, MPU_axis* dataStruct) {
	uint8_t buffer[6];
	I2C_burstRead(i2cx, MPU_address, MPU_startRegister, buffer, 6);

	dataStruct->X = (int16_t)((buffer[0] << 8) | buffer[1]);
	dataStruct->Y = (int16_t)((buffer[2] << 8) | buffer[3]);
	dataStruct->Z = (int16_t)((buffer[4] << 8) | buffer[5]);
}

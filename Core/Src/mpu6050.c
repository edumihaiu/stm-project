/*
 * mpu6050.c
 *
 *  Created on: Dec 23, 2025
 *      Author: edu
 */

#include "mpu6050.h"
#include "i2c_driver.h"



void MPU_init(I2C_TypeDef* i2cx)
{
	I2C_init(i2cx);
	I2C_start(i2cx);
	I2C_sendAddr(i2cx, MPU_address << 1);
	I2C_writeData(i2cx, MPU_powerMgmtRegister);
	I2C_writeData(i2cx, 0x00);
	I2C_stop(i2cx);
}

static uint8_t dma_buffer[6]; // Raw storage for Accel(6)

I2C_Status MPU_parseData(I2C_TypeDef* i2cx, MPU_axis* dataStruct)
{
	uint8_t buffer[6];
	if(I2C_burstRead(i2cx, MPU_address, MPU_Accel_X_High, buffer, 6) != I2C_OK) return I2C_TIMEOUT;

	dataStruct->X = (int16_t)((buffer[0] << 8) | buffer[1]);
	dataStruct->Y = (int16_t)((buffer[2] << 8) | buffer[3]);
	dataStruct->Z = (int16_t)((buffer[4] << 8) | buffer[5]);
}

I2C_Status MPU_startReadDMA(I2C_TypeDef* i2cx) {
    return I2C_burstRead_DMA(i2cx, MPU_address, MPU_Accel_X_High, dma_buffer, 6);
}

I2C_Status MPU_parseDataDMA(I2C_TypeDef* i2cx, MPU_axis* dataStruct)
{
	dataStruct->X = (int16_t)((dma_buffer[0] << 8) | dma_buffer[1]);
	dataStruct->Y = (int16_t)((dma_buffer[2] << 8) | dma_buffer[3]);
	dataStruct->Z = (int16_t)((dma_buffer[4] << 8) | dma_buffer[5]);
}



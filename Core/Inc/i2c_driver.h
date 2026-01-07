#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32f4xx.h"

typedef enum {
	I2C_OK=0,
	I2C_ERROR=1,
	I2C_TIMEOUT=2
} I2C_Status;

void I2C_init(I2C_TypeDef* i2cx);
void I2C_start(I2C_TypeDef* i2cx);
void I2C_writeData(I2C_TypeDef* i2cx, uint8_t data);
void I2C_stop(I2C_TypeDef* i2cx);
void I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr);
void I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size);

#endif
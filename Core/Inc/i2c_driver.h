#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f4xx.h"

void I2C_init(I2C_TypeDef* i2cx);
void I2C_start(I2C_TypeDef* i2cx);
void I2C_writeData(I2C_TypeDef* i2cx, uint8_t data);
void I2C_stop(I2C_TypeDef* i2cx);
void I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr);
void I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size);

#endif
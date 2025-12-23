#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f4xx.h"

void I2C1_init(void);
void I2C1_start(void);
void I2C1_writedata(uint8_t data);
void I2C1_stop(void);
void I2C1_sendaddr(uint8_t addr);

#endif
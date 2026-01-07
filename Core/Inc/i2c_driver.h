#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include "stm32f4xx.h"


// DEBUG MODE - DO NOT USE IF NOT DEBUGGING
#define DEBUG_MODE

typedef enum {
	I2C_OK=0,
	I2C_ERROR=1,
	I2C_TIMEOUT=2
} I2C_Status;

#ifdef DEBUG_MODE
    #define I2C_ASSERT(x)  do { \
                            if ((x) != I2C_OK) { \
                                __BKPT(0); \
                            } \
                          } while(0)
#else
    #define I2C_ASSERT(x)  (x)
#endif

void I2C_init(I2C_TypeDef* i2cx);
void I2C_stop(I2C_TypeDef* i2cx);
I2C_Status I2C_start(I2C_TypeDef* i2cx);
I2C_Status I2C_writeData(I2C_TypeDef* i2cx, uint8_t data);
I2C_Status I2C_sendAddr(I2C_TypeDef* i2cx, uint8_t addr);
void I2C_burstRead(I2C_TypeDef* i2cx, uint8_t slaveAddress, uint8_t startReg, uint8_t* buffer, uint16_t size);

#endif
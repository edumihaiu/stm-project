/*
 * uart_driver.h
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

#include <stdint.h>
#include "mpu6050.h"

void UART_init(void);
void UART_write(char c);
void UART_print(char* str);
void UART_printNumber(int16_t num);
void UART_printXYZ(MPU_axis* data);

#endif /* INC_UART_DRIVER_H_ */

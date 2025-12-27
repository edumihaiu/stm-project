/*
 * uart_driver.h
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

#include "stm32f4xx.h"

#define AF7 0x7

void UART_init(void);
void UART_write(char c);
void UART_print(char* str);
void UART_printNumber(int16_t num);

#endif /* INC_UART_DRIVER_H_ */

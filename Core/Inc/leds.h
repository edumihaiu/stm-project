/*
 * leds.h
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "imu_math.h"
#include "stm32f4xx.h"
#include "uart_driver.h"


void leds_init(void);
void leds_update(float angle);


#endif /* INC_LEDS_H_ */

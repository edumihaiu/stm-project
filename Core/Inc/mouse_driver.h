/*
 * mouse_driver.h
 *
 *  Created on: Jan 12, 2026
 *      Author: edu
 */

#ifndef INC_MOUSE_DRIVER_H_
#define INC_MOUSE_DRIVER_H_

#include "mpu6050.h"


void mouse_init(void);
void mouse_logic(MPU_axis* data);


#endif /* INC_MOUSE_DRIVER_H_ */

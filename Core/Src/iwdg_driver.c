/*
 * iwdg_driver.c
 *
 *  Created on: Jan 11, 2026
 *      Author: edu
 */

#include "iwdg_driver.h"
#include "stm32f4xx.h"

void IWDG_init(void)
{
	IWDG->KR = 0x5555;

	IWDG->PR = 0x06; // prescaling /256

	IWDG->RLR = 125U;

	IWDG->KR = 0xCCCC; // start
}

void IWDG_refresh(void)
{
	IWDG->KR = 0xAAAA; // refresh
}

/*
 * leds.c
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#include "leds.h"

void leds_init(int8_t numOfLeds, int8_t startPortNum) // portb leds
{
	GPIOB->MODER &= ~(0x3F); // reset Pb0,1,2
	GPIOB->MODER |= (1U << 0);
	GPIOB->MODER |= (1U << 2);
	GPIOB->MODER |= (1U << 4);
}
void leds_update(float angle)
{
    if (angle < -90) angle = -90;
    if (angle > 90)  angle = 90;

    int index = (int)((angle + 30.0f) / 60.0f);

    if (index > 2) index = 2;
    if (index < 0) index = 0;

    GPIOB->ODR &= ~(7U);

    // Pasul B: Aprindem DOAR led-ul calculat
    // Shiftam 1 la stanga cu 'index' pozitii
    GPIOB->ODR |= (1 << index);
}

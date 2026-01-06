/*
 * leds.c
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */

#include "leds.h"
uint8_t led_map[7] = {4, 3, 0, 1, 2, 5, 8};
void leds_init(void) // portb leds
{
	GPIOB->MODER &= ~(0xFFF); // reset Pb0,1,2
	GPIOB->MODER &= ~(3U << 16);
	for (int i = 0; i < 7; i++)
	{
		uint8_t led = led_map[i];
		GPIOB->MODER |= (1U << (led*2));
	}
}
void leds_update(float angle)
{
	UART_print("angle: ");
	UART_printNumber(angle);
    if (angle < -90) angle = -90;
    if (angle > 90)  angle = 90;
    float step = 2.0f;

    int index = (int)(angle / step) + 3;

    if (index > 6) index = 6;
    if (index < 0) index = 0;

    uint8_t pin = led_map[index];
    GPIOB->ODR &= ~(0x13F);


    // Pasul B: Aprindem DOAR led-ul calculat
    // Shiftam 1 la stanga cu 'index' pozitii
    GPIOB->ODR |= (1 << pin);
}

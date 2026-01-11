/*
 * uart.c
 *
 *  Created on: Dec 27, 2025
 *      Author: edu
 */


#include "uart_driver.h"
#include "stm32f4xx.h"

#define AF7 0x7

void UART_init(void) // USART1 PA9 PA10
{
	RCC->AHB1ENR |= (1 << 0); // enable clock PORTA
	RCC->APB2ENR |= (1 << 4); // enable clock USART1

	// setting RX, TX to alternate function mode
	GPIOA->MODER &= ~(0xF << 18); // reset PA9,10
	GPIOA->MODER |= (2U << 18); // PA9 AF
	GPIOA->MODER |= (2U << 20); // PA10 AF

	// setting RX, TX to high speed outputs
	GPIOA->OSPEEDR &= ~(0xF << 18);
	GPIOA->OSPEEDR |= (3U << 18);
	GPIOA->OSPEEDR |= (3U << 20);

	// setting RX, TX to AF7 ( USART1)
	GPIOA->AFR[1] &= ~(0xFF << 4);
	GPIOA->AFR[1] |= (AF7 << 4);
	GPIOA->AFR[1] |= (AF7 << 8);

	//setting baud rate formula: clock / baud rate
	USART1->BRR |= (0x8B << 0);
	USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	// en DMA
	USART1->CR3 |= USART_CR3_DMAT;
}

void UART_write(char c)
{
	while (!(USART1->SR & (1 << 7) ));
	USART1->DR = c;
}

void UART_print(char* str)
{
	while(*str) UART_write(*str++);
}

void UART_printNumber(int16_t num)
{
	char buffer[20];
	if (num == 0)
	{
		UART_write('0');
		return;
	}
	if (num < 0)
	{
		UART_write('-');
		num = -num;
	}
	int i = 0;
	while (num)
	{
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}
	while(i--) UART_write(buffer[i]);
}

void UART_printXYZ(MPU_axis* data)
{
	UART_print("-------------\r\n");
	UART_printNumber(data->X);
	UART_write('\n');
	UART_printNumber(data->Y);
	UART_write('\n');
	UART_printNumber(data->Z);
	UART_write('\n');
	UART_print("-------------\r\n");
}


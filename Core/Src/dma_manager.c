/*
 * dma_manager.c
 *
 *  Created on: Jan 7, 2026
 *      Author: edu
 */
#include "dma_manager.h"
#include "stm32f4xx.h"

#define DMA_TIMEOUT 500000

void DMA_Manager_init(void) {
	RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN); // enable DMA1 and DMA2
}

void DMA_Config_I2C1_RX(uint8_t* buffer, uint16_t size)
{
	// disable stream 0
	DMA1_Stream0->CR &= ~(DMA_SxCR_EN);
	while (DMA1_Stream0->CR & DMA_SxCR_EN);

	// reset
	DMA1_Stream0->CR = 0;

	// clean old flags
	DMA1->LIFCR |= (DMA_LIFCR_CTCIF0 | DMA_LIFCR_CTEIF0);

	// addresses setup
	DMA1_Stream0->PAR = (uint32_t)&I2C1->DR;
	DMA1_Stream0->M0AR = (uint32_t)buffer;
	DMA1_Stream0->NDTR = size;

	DMA1_Stream0->CR = (1U << DMA_SxCR_CHSEL_Pos); // channel select
    DMA1_Stream0->CR |= DMA_SxCR_MINC;             // memory increment after each data transfer
	DMA1_Stream0->CR |= (3U << DMA_SxCR_PL_Pos);   // very high priority level

	// enable dma1
	DMA1_Stream0->CR |= DMA_SxCR_EN;
}

void DMA_Config_USART1_TX(char* str, uint16_t size)
{
	// disable stream 7
	DMA2_Stream7->CR &= ~(DMA_SxCR_EN);
	while (DMA2_Stream7->CR & DMA_SxCR_EN);

	// clean old flags
	DMA2->HIFCR |= (DMA_HIFCR_CTCIF7 | DMA_HIFCR_CTEIF7);

	// addresses setup
	DMA2_Stream7->PAR = (uint32_t)&(USART1->DR);
	DMA2_Stream7->M0AR = (uint32_t)str;
	DMA2_Stream7->NDTR = size;

	DMA2_Stream7->CR |= (4U << DMA_SxCR_CHSEL_Pos); // channel select
	DMA2_Stream7->CR |= DMA_SxCR_DIR_0; 		    // memory-to-peripheral
    DMA2_Stream7->CR |= DMA_SxCR_MINC;              // memory increment after each data transfer
	DMA2_Stream7->CR |= (3U << DMA_SxCR_PL_Pos);    // very high priority level

	// enable dma2
	DMA2_Stream7->CR |= DMA_SxCR_EN;
}

uint8_t DMA_I2C1_IsComplete(void) {
    if (DMA1->LISR & DMA_LISR_TCIF0) {
        DMA1->LIFCR |= DMA_LIFCR_CTCIF0;
		I2C1->CR1 |= I2C_CR1_STOP;
        return 1;
    }
    return 0;
}

uint8_t DMA_USART1_IsComplete(void) {
    if (DMA2->HISR & DMA_HISR_TCIF7) {
        DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
        return 1;
    }
    return 0;
}

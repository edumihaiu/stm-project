/*
 * dma_manager.c
 *
 *  Created on: Jan 7, 2026
 *      Author: edu
 */
#include "dma_manager.h"
#include "stm32f4xx.h"

void DMA_Manager_init(void) {
	RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN); // enable DMA1 and DMA2
}


/*
 * dma_manager.h
 *
 *  Created on: Jan 7, 2026
 *      Author: edu
 */

#ifndef INC_DMA_MANAGER_H_
#define INC_DMA_MANAGER_H_

#include <stdint.h>

typedef enum {
	DMA_OK=0,
	DMA_ERROR=1,
	DMA_TIMEOUT=2
} DMA_Status;

void DMA_Manager_init(void);
void DMA_Config_USART1_TX(char* str, uint16_t size);
void DMA_Config_I2C1_RX(uint8_t* buffer, uint16_t size);
uint8_t DMA_I2C1_IsComplete(void);
uint8_t DMA_USART1_IsComplete(void);

#endif /* INC_DMA_MANAGER_H_ */

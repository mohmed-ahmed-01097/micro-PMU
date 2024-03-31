/*
 * nvic.h
 *
 *  Created on: Jul 31, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_NVIC_H_
#define INC_NVIC_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BUSY_IRQ					EXTI15_10_IRQn

#define DMA2_MEMORY_STREAM_IRQ		DMA2_Stream0_IRQn

#define USART2_IRQ					USART2_IRQn
#define DMA1_USART2_RX_STREAM_IRQ	DMA1_Stream5_IRQn
#define DMA1_USART2_TX_STREAM_IRQ	DMA1_Stream6_IRQn

#define USER_BUTTON_IRQ				EXTI0_IRQn

#define uPMU_PROCESS_IRQ			EXTI1_IRQn

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uPMU_NVIC_Init(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_NVIC_H_ */

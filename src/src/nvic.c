/*
 * nvic.c
 *
 *  Created on: Jul 31, 2021
 *      Author: MAAM
 */

/* Includes */
#include "nvic.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** NVIC IRQs initialization function **/
void uPMU_NVIC_Init(void){

	/* Set priority group to 3
	 * bits[3:0] are the sub-priority,
	 * bits[7:4] are the pre-empt priority  (0-15) */
	NVIC_SetPriorityGrouping(3);

	/* Set priority levels */
	NVIC_SetPriority(BUSY_IRQ					, 1);
	NVIC_SetPriority(DMA2_MEMORY_STREAM_IRQ		, 2);
	NVIC_SetPriority(DMA1_USART2_TX_STREAM_IRQ	, 3);
	NVIC_SetPriority(DMA1_USART2_RX_STREAM_IRQ	, 4);
	NVIC_SetPriority(USART2_IRQ					, 5);
	NVIC_SetPriority(USER_BUTTON_IRQ			, 14);
	NVIC_SetPriority(uPMU_PROCESS_IRQ			, 15);

	/* Enable interrupts at NVIC */
	NVIC_EnableIRQ(BUSY_IRQ);
	NVIC_EnableIRQ(DMA2_MEMORY_STREAM_IRQ);
	NVIC_EnableIRQ(DMA1_USART2_TX_STREAM_IRQ);
	NVIC_EnableIRQ(DMA1_USART2_RX_STREAM_IRQ);
	NVIC_EnableIRQ(USART2_IRQ);
	NVIC_EnableIRQ(USER_BUTTON_IRQ);
	NVIC_EnableIRQ(uPMU_PROCESS_IRQ);


}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

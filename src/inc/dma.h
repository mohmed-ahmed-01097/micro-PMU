/*
 * dma.h
 *
 *  Created on: Aug 4, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_DMA_H_
#define INC_DMA_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DMA2_Stream0_Init(void);
void DMA2_Stream0_Enable(void);
void DMA2_Stream0_Set_Addresses(const uint32_t * src, uint32_t * dst, const uint32_t size);

//void DMA1_Stream5_Init(void);
//void DMA1_Stream5_Enable(void);
//void DMA1_Stream5_Set_Addresses(const uint32_t * src, uint32_t * dst, const uint32_t size);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_DMA_H_ */

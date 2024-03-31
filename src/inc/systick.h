/*
 * systick.h
 *
 *  Created on: Aug 2, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if (FREQUENCY == 50U)
	#define SYSTICK_CONFIG0					6562	 //(SystemCoreClock / 1000)
	#define SYSTICK_CONFIG1					6561	 // 6562.5  => (SystemCoreClock / 1000)/(512*50*0.001)
	#define SYSTICK_ACC						2
#endif
#if (FREQUENCY == 60U)
	#define SYSTICK_CONFIG0					5468	 //(SystemCoreClock / 1000)
	#define SYSTICK_CONFIG1					5467	 // 5468.75  => (SystemCoreClock / 1000)/(512*60*0.001)
	#define SYSTICK_ACC						4
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SysTick_Init(void);
void SysTick_IncrementTicks_CallBack(void);
void SysTick_Delay(uint32_t wait_time_ms);
uint32_t SysTick_GetCurrentTick(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTICK_H_ */

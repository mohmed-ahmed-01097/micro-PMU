/*
 * systick.c
 *
 *  Created on: Aug 2, 2021
 *      Author: MAAM
 */

/* Includes */
#include "systick.h"
#include "gpio.h"

volatile uint32_t SysTickCounter = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** SysTick initial configuration **/
void SysTick_Init(void) {

	/* Update clock configuration */
	SystemCoreClockUpdate();

	/* Check clock configuration */
	if(SystemCoreClock != CPU_PLL_CLK) {
		while(1);	/* Clock configuration is not OK */
	}

	/* Configure SysTick to generate an interrupt every millisecond */
	/* Check return code for errors */
	if (SysTick_Config(SYSTICK_CONFIG0)) {
		while(1);	/* SysTick configuration failed */
	}

	/* Give SysTick interrupts highest priority */
	NVIC_SetPriority(SysTick_IRQn, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** increment SysTick counter callback **/
void SysTick_IncrementTicks_CallBack(void) {

	/* Falling Edge of the Convert Signal */
	ODR_CON_CV_BITBAND = RESET;

	/* Accumulate the SysTick Timer Clock */
	if(!((++SysTickCounter) % SYSTICK_ACC)){
		SysTick->LOAD  = SYSTICK_CONFIG1;
	}else{
		SysTick->LOAD  = SYSTICK_CONFIG0;
	}

	/* Rising Edge of the Convert Signal */
	ODR_CON_CV_BITBAND = SET;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Delay function based on SysTick **/
void SysTick_Delay(uint32_t wait_time_ms) {

	/* Store start tick */
	uint32_t startTick = SysTickCounter;

	/* Loop until timeout */
	while((SysTickCounter - startTick) < wait_time_ms);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Return current SysTick counter **/
uint32_t SysTick_GetCurrentTick(void) {
	return(SysTickCounter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

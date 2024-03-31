/*
 * exti.c
 *
 *  Created on: Jul 31, 2021
 *      Author: MAAM
 */

/* Includes */
#include "exti.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** EXTI Configuration function **/
void uPMU_EXTIx_Init(uint8_t port, uint16_t bit, EXTI_Trigger_TypeDef triger){

	/* Check if the bit number if available*/
	if(bit > 22)
		return;

	/* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2LPENR_SYSCFGLPEN;

	/* Map port bit to EXTx */
	SYSCFG->EXTICR[bit >> 0x02] &= ~((uint32_t)0x0F) << (4 * (bit & 0x03));
	SYSCFG->EXTICR[bit >> 0x02] |=  ((uint32_t)port) << (4 * (bit & 0x03));

	switch(triger){
	case EXTI_Rising:
		/* Disable falling edge trigger */
		EXTI->FTSR &= ~(uint32_t)(1U << bit);
		/* Enable rising edge trigger */
		EXTI->RTSR |=  (uint32_t)(1U << bit);
		break;
	case EXTI_Falling:
		/* Disable rising edge trigger */
		EXTI->RTSR &= ~(uint32_t)(1U << bit);
		/* Enable falling edge trigger */
		EXTI->FTSR |=  (uint32_t)(1U << bit);
		break;
	case EXTI_Rising_Falling:
		/* Enable rising edge trigger */
		EXTI->RTSR |=  (uint32_t)(1U << bit);
		/* Enable falling edge trigger */
		EXTI->FTSR |=  (uint32_t)(1U << bit);
		break;
	}

	/* Clear Registers */
	EXTI->IMR &= ~(uint32_t)(1U << bit);
	EXTI->EMR &= ~(uint32_t)(1U << bit);

	/* Enable interrupt line */
	EXTI->IMR |=  (uint32_t)(1U << bit);
	/* Enable event line */
	EXTI->EMR |=  (uint32_t)(1U << bit);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Check if the EXCTI Line is Pending and IT **/
FlagStatus EXTI_Line_Check(uint32_t EXTI_Line) {

	if ((EXTI->PR & EXTI_Line) && (EXTI->IMR & EXTI_Line)){
		return SET;
	}
	return RESET;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Clear EXTI Pending Bit **/
void EXTI_Clear_PendingBit(uint32_t EXTI_Line){

	  EXTI->PR = EXTI_Line;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Set EXTI Pending Bit **/
void EXTI_Set_PendingBit(uint32_t EXTI_Line){

	EXTI->SWIER |= EXTI_Line;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

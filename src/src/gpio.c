/*
 * gpio.c
 *
 *  Created on: Jul 28, 2021
 *      Author: MAAM
 */

/* Includes */
#include "gpio.h"
#include "exti.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** uPM GPIO initialization  **/
void uPMU_GPIO_Init(void){

	Data_Port_Init   (   DATA_GPIO_PORT,    DATA_GPIO_RCC_AHB1ENR);
	Control_Pins_Init(CONTROL_GPIO_PORT, CONTROL_GPIO_RCC_AHB1ENR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Data Port Configuration function **/
void Data_Port_Init(GPIO_TypeDef* GPIOx, uint32_t RCC_AHB1ENR_GPIOx){

	/* Enable AHB1ENR GPIOx clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOx;
	asm("NOP");		/* wait to Enable */

	/* GPIO Mode as Input*/
	GPIOx->MODER  = Mode_IN_ALL;
	/* GPIO no Pull */
	GPIOx->PUPDR  = PUPDR_NO_ALL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Control pins Configuration function **/
void Control_Pins_Init(GPIO_TypeDef* GPIOx, uint32_t RCC_AHB1ENR_GPIOx){

	/* Enable AHB1ENR GPIOx clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOx;

	GPIO_Bit_Init_Input(GPIOx, CON_AFRST);
	GPIO_Bit_Init_Input(GPIOx, CON_BFRST);

	GPIO_Bit_Init_Input(GPIOx, CON_ABUSY);
	GPIO_Bit_Init_Input(GPIOx, CON_BBUSY);

	uPMU_EXTIx_Init(EXTI_CONT_Port_Source, CON_ABUSY, EXTI_Falling);
	uPMU_EXTIx_Init(EXTI_CONT_Port_Source, CON_BBUSY, EXTI_Falling);

	GPIO_Bit_Init_Output(GPIOx, CON_CS);
	GPIO_Bit_Init_Output(GPIOx, CON_SE);
	GPIO_Bit_Init_Output(GPIOx, CON_RST);

	//GPIO_Bit_Init_AF(GPIOx, CON_CV,	CON_CV_AF);
	GPIO_Bit_Init_Output(GPIOx, CON_CV);
	ODR_CON_CV_BITBAND = SET;


	ODR_CON_RST_BITBAND = SET;
	ODR_CON_CS_BITBAND = SET;
	ODR_CON_SE_BITBAND = SET;
	ODR_CON_RST_BITBAND = RESET;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Bit Configuration function as input **/
uint8_t GPIO_Bit_Init_Input(GPIO_TypeDef* GPIOx, uint32_t bit){

	bit *= 2;			/* wait to Enable*/

	/* GPIO Mode as Input*/
	GPIOx->MODER   &= ~(Mode_AN		<<bit);
	GPIOx->MODER   |=  (Mode_IN		<<bit);
	/* GPIO no Pull */
	GPIOx->PUPDR   &= ~(PUPDR_RST	<<bit);
	GPIOx->PUPDR   |=  (PUPDR_NO	<<bit);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Bit Configuration function as output **/
uint8_t GPIO_Bit_Init_Output(GPIO_TypeDef* GPIOx, uint32_t bit){

	bit *= 2;			/* wait to Enable*/

	/* GPIO Mode as Output*/
	GPIOx->MODER   &= ~(Mode_AN		<<bit);
	GPIOx->MODER   |=  (Mode_OUT	<<bit);

	/* GPIO Output Type as Push Pull */
	(PERIPH_BITBAND(((uint32_t)(&GPIOx->OTYPER)), (bit/2))) = OTYPER_PP;

	/* GPIO Output Very Speed = 50MHz*/
	GPIOx->OSPEEDR |= (OSPEEDR_VERY	<<bit);
	/* Check APB2ENR clock is Enable */
	if((RCC->APB2ENR & RCC_APB2LPENR_SYSCFGLPEN) != RCC_APB2LPENR_SYSCFGLPEN){
		RCC->APB2ENR   |= RCC_APB2LPENR_SYSCFGLPEN;
		SYSCFG->CMPCR  |= SYSCFG_CMPCR_CMP_PD;
		while((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) != SYSCFG_CMPCR_READY);
	}

	/* GPIO no Pull */
	GPIOx->PUPDR   &= ~(PUPDR_RST	<<bit);
	GPIOx->PUPDR   |=  (PUPDR_NO	<<bit);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Bit Configuration function as Alternative function **/
uint8_t GPIO_Bit_Init_AF(GPIO_TypeDef* GPIOx, uint32_t bit, uint8_t AFx){

	bit *= 2;			/* wait to Enable*/

	/* GPIO Mode as AF*/
	GPIOx->MODER   &= ~(Mode_AN		<<bit);
	GPIOx->MODER   |=  (Mode_AF		<<bit);

	/* GPIO Output Type as Push Pull */
	(PERIPH_BITBAND(((uint32_t)(&GPIOx->OTYPER)), (bit/2))) = OTYPER_PP;

	/* GPIO AFR[] register */
	GPIOx->AFR[(bit >> 4)] |= ((uint32_t)AFx << (4 * ((bit/2) & 0x7)));

	/* GPIO Output Very Speed = 50MHz*/
	GPIOx->OSPEEDR |= (OSPEEDR_VERY	<<bit);
	/* Check APB2ENR clock is Enable */
	if((RCC->APB2ENR & RCC_APB2LPENR_SYSCFGLPEN) != RCC_APB2LPENR_SYSCFGLPEN){
		RCC->APB2ENR   |= RCC_APB2LPENR_SYSCFGLPEN;
		SYSCFG->CMPCR  |= SYSCFG_CMPCR_CMP_PD;
		while((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) != SYSCFG_CMPCR_READY);
	}

	/* GPIO no Pull */
	GPIOx->PUPDR   &= ~(PUPDR_RST	<<bit);
	GPIOx->PUPDR   |=  (PUPDR_NO	<<bit);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

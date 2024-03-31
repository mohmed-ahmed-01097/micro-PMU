/*
 * led.c
 *
 *  Created on: Jul 29, 2021
 *      Author: MAAM
 */

#include "led.h"
#include "exti.h"

volatile uint8_t Error_Code = 0x00;
const uint8_t LEDS[LEDn] = {LED3,LED5,LED6,LED4};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** On-board initialization function **/
void OnBoard_Init(void){

	GPIO_Init_LED(LED3);
	GPIO_Init_LED(LED4);
	GPIO_Init_LED(LED5);
	GPIO_Init_LED(LED6);
	GPIO_Init_BUTTON(BUTTON_USER, EXTI0_IRQn);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** LED initialization function **/
void GPIO_Init_LED(Led_TypeDef led){

	/* Enable AHB1ENR clock */
	RCC->AHB1ENR   |= LED_GPIO_RCC_AHB1ENR;
	led *= 2;		/* wait to Enable */

	/* Configure the GPIO_LED pin */
	/* GPIO Mode as Output*/
	LED_GPIO_PORT->MODER   &= ~(Mode_AN		<<led);
	LED_GPIO_PORT->MODER   |=  (Mode_OUT	<<led);

	/* GPIO Output Type as Push Pull */
	(PERIPH_BITBAND(((uint32_t)(&LED_GPIO_PORT->OTYPER)), (led/2))) = OTYPER_PP;

	/* GPIO Output Very Speed = 50MHz*/
	LED_GPIO_PORT->OSPEEDR |= (OSPEEDR_VERY	<<led);
	/* Check APB2ENR clock is Enable */
	if((RCC->APB2ENR & RCC_APB2LPENR_SYSCFGLPEN) != RCC_APB2LPENR_SYSCFGLPEN){
		/* Enable APB2ENR clock */
		RCC->APB2ENR   |= RCC_APB2LPENR_SYSCFGLPEN;
		/* Enable Compensation cell */
		SYSCFG->CMPCR  |= SYSCFG_CMPCR_CMP_PD;
		/* wait Compensation cell to be Ready */
		while((SYSCFG->CMPCR & SYSCFG_CMPCR_READY) != SYSCFG_CMPCR_READY);
	}

	/* GPIO no Pull */
	LED_GPIO_PORT->PUPDR   &= ~(PUPDR_RST		<<led);
	LED_GPIO_PORT->PUPDR   |=  (PUPDR_PU		<<led);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Write state to the led **/
void GPIO_Write_LED(Led_TypeDef led, Output_Typedef state){

	switch(led){
	case LED3:
		ODR_LED3_BITBAND = state;
		break;
	case LED4:
		ODR_LED4_BITBAND = state;
		break;
	case LED5:
		ODR_LED5_BITBAND = state;
		break;
	case LED6:
		ODR_LED6_BITBAND = state;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Read the state of led **/
uint8_t GPIO_Read_LED(Led_TypeDef led){

	switch(led){
	case LED3:
		return ODR_LED3_BITBAND;
		break;
	case LED4:
		return ODR_LED4_BITBAND;
		break;
	case LED5:
		return ODR_LED5_BITBAND;
		break;
	case LED6:
		return ODR_LED6_BITBAND;
		break;
	}
	return Low;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** BUTTON initialization function **/
void GPIO_Init_BUTTON(Button_TypeDef button, uint16_t IRQx){

	/* Enable BUTTON_GPIO_PORT AHB1ENR Clock */
	RCC->AHB1ENR   |= BUTTON_GPIO_RCC_AHB1ENR ;
	button *= 2;			/* wait to Enable*/

	/* GPIO Mode as Input*/
	BUTTON_GPIO_PORT->MODER   &= ~(Mode_AN		<<button);
	BUTTON_GPIO_PORT->MODER   |=  (Mode_IN		<<button);
	/* GPIO no Pull */
	BUTTON_GPIO_PORT->PUPDR   &= ~(PUPDR_RST	<<button);
	BUTTON_GPIO_PORT->PUPDR   |=  (PUPDR_NO		<<button);

	/*get back button pin number*/
	button /= 2;

	/* EXTI0 Configuration */
	uPMU_EXTIx_Init(SYSCFG_EXTICR1_EXTI0_PA, button, EXTI_Rising);
	uPMU_EXTIx_Init(SYSCFG_EXTICR1_EXTI1_PA, uPMU_PROCESS, EXTI_Rising);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Error Code Finite State Machine Display **/
void uPMU_Error_Code_Display(void){

	static uint8_t i = 0;		/* display index*/

	if(!(i) && !EXTI_Line_Check(BUTTON_USER_Line)) {
		EXTI_Set_PendingBit(BUTTON_USER_Line);
	}

	switch(Error_Code % ERRORn){
	case 0:
		GPIO_Write_LED(LEDS[  i%LEDn], Low);
		GPIO_Write_LED(LEDS[++i%LEDn], High);
		break;
	case 1:
		GPIO_Write_LED(LEDS[--i%LEDn], Low);
		GPIO_Write_LED(LEDS[++i%LEDn], High);
		GPIO_Write_LED(LEDS[++i%LEDn], High);
		break;
	case 2:
		GPIO_Write_LED(LEDS[  i%LEDn], High);
		GPIO_Write_LED(LEDS[++i%LEDn], Low);
		break;
	case 3:
		GPIO_Write_LED(LEDS[--i%LEDn], High);
		GPIO_Write_LED(LEDS[++i%LEDn], Low);
		GPIO_Write_LED(LEDS[++i%LEDn], Low);
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Set Error Code **/
void uPMU_Set_Error_Code(uint8_t code){
	Error_Code = code;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Get Error Code **/
uint8_t uPMU_Get_Error_Code(void){
	return Error_Code;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

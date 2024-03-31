/*
 * gpio.h
 *
 *  Created on: Jul 28, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_GPIO_H_
#define INC_GPIO_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum {
	CON_CS = 8, CON_CV, CON_SE, CON_ABUSY, CON_BBUSY, CON_AFRST, CON_BFRST, CON_RST,
} CON_TypeDef;


typedef enum {
	Low = 0L, High = 1L,
} Output_Typedef;

typedef enum {
	Mode_IN, Mode_OUT, Mode_AF, Mode_AN,
} MODE_TypeDef;

#define Mode_IN_ALL			((uint32_t)0x00000000)
#define Mode_OUT_ALL		((uint32_t)0x55555555)
#define Mode_AF_ALL			((uint32_t)0xAAAAAAAA)
#define Mode_AN_ALL			((uint32_t)0xFFFFFFFF)

typedef enum {
	OTYPER_PP, OTYPER_OD,
} OTYPER_Typedef;

typedef enum {
	OSPEEDR_LOW, OSPEEDR_MEDIUM, OSPEEDR_HIGH, OSPEEDR_VERY,
} OSPEEDR_Typedef;

typedef enum {
	PUPDR_NO, PUPDR_PU, PUPDR_PD, PUPDR_RST
} PUPDR_Typedef;

#define PUPDR_NO_ALL		((uint32_t)0x00000000)
#define PUPDR_PU_ALL		((uint32_t)0x55555555)
#define PUPDR_PD_ALL		((uint32_t)0xAAAAAAAA)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DATA_GPIO_PORT				GPIOC
#define DATA_GPIO_RCC_AHB1ENR		RCC_AHB1ENR_GPIOCEN

#define CONTROL_GPIO_PORT			GPIOE
#define CONTROL_GPIO_RCC_AHB1ENR	RCC_AHB1ENR_GPIOEEN
#define EXTI_CONT_Port_Source		SYSCFG_EXTICR1_EXTI0_PE

#define CON_CV_AF 					0x1U		// AF1 Timer1/2

#define CONTROL_GPIO_ODR_ADD        ((uint32_t)(&CONTROL_GPIO_PORT->ODR))
#define ODR_CON_CS_BITBAND			(PERIPH_BITBAND(CONTROL_GPIO_ODR_ADD, CON_CS))
#define ODR_CON_SE_BITBAND			(PERIPH_BITBAND(CONTROL_GPIO_ODR_ADD, CON_SE))
#define ODR_CON_RST_BITBAND			(PERIPH_BITBAND(CONTROL_GPIO_ODR_ADD, CON_RST))

#define CONTROL_GPIO_IDR_ADD        ((uint32_t)(&CONTROL_GPIO_PORT->IDR))
#define IDR_CON_AFRST_BITBAND		(PERIPH_BITBAND(CONTROL_GPIO_IDR_ADD, CON_AFRST))
#define IDR_CON_BFRST_BITBAND		(PERIPH_BITBAND(CONTROL_GPIO_IDR_ADD, CON_BFRST))

#define ODR_CON_CV_BITBAND			(PERIPH_BITBAND(CONTROL_GPIO_ODR_ADD, CON_CV))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uPMU_GPIO_Init(void);

void Data_Port_Init(GPIO_TypeDef* GPIOx, uint32_t RCC_AHB1ENR_GPIOx);
void Control_Pins_Init(GPIO_TypeDef* GPIOx, uint32_t RCC_AHB1ENR_GPIOx);

uint8_t GPIO_Bit_Init_Input(GPIO_TypeDef* GPIOx, uint32_t bit);
uint8_t GPIO_Bit_Init_Output(GPIO_TypeDef* GPIOx, uint32_t bit);
uint8_t GPIO_Bit_Init_AF(GPIO_TypeDef* GPIOx, uint32_t bit, uint8_t AFx);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_GPIO_H_ */

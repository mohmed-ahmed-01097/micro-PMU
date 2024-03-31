/*
 * led.h
 *
 *  Created on: Jul 29, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_LED_H_
#define INC_LED_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"
#include "gpio.h"

typedef enum {
	LED4 = 12, LED3 = 13, LED5 = 14, LED6 = 15,
} Led_TypeDef;

typedef enum {
	BUTTON_USER, uPMU_PROCESS
} Button_TypeDef;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LEDn                          	4

#if (FREQUENCY == 50U)
	#define SYSTICK_ERROR_CODE_DELAY		3200	//125
#endif
#if (FREQUENCY == 60U)
	#define SYSTICK_ERROR_CODE_DELAY		3840	//125
#endif

#define BUTTON_GPIO_PORT				GPIOA
#define BUTTON_GPIO_RCC_AHB1ENR			RCC_AHB1ENR_GPIOAEN

#define LED_GPIO_PORT					GPIOD
#define LED_GPIO_RCC_AHB1ENR			RCC_AHB1ENR_GPIODEN

#define LED_GPIO_ODR_ADD              	((uint32_t)(&LED_GPIO_PORT->ODR))

#define ODR_LED3_BITBAND				(PERIPH_BITBAND(LED_GPIO_ODR_ADD, LED3))
#define ODR_LED4_BITBAND				(PERIPH_BITBAND(LED_GPIO_ODR_ADD, LED4))
#define ODR_LED5_BITBAND				(PERIPH_BITBAND(LED_GPIO_ODR_ADD, LED5))
#define ODR_LED6_BITBAND				(PERIPH_BITBAND(LED_GPIO_ODR_ADD, LED6))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnBoard_Init(void);

void 	GPIO_Init_LED (Led_TypeDef led);
void 	GPIO_Write_LED(Led_TypeDef led, Output_Typedef state);
uint8_t GPIO_Read_LED (Led_TypeDef led);

void GPIO_Init_BUTTON(Button_TypeDef button, uint16_t IRQx);

void 	uPMU_Error_Code_Display(void);
void 	uPMU_Set_Error_Code(uint8_t code);
uint8_t uPMU_Get_Error_Code(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif


#endif /* INC_LED_H_ */

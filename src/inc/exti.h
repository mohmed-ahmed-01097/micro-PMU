/*
 * exti.h
 *
 *  Created on: Jul 31, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_EXTI_H_
#define INC_EXTI_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "gpio.h"
#include "led.h"

typedef enum {
  EXTI_Rising, EXTI_Falling, EXTI_Rising_Falling,
}EXTI_Trigger_TypeDef;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BUTTON_USER_Line       	(1UL << BUTTON_USER)    /*!< External interrupt line 0 */

#define uPMU_PROCESS_Line      	(1UL << uPMU_PROCESS)   /*!< External interrupt line 1 */

#define CON_ABUSY_Line       	(1UL << CON_ABUSY)     	/*!< External interrupt line x */
#define CON_BBUSY_Line       	(1UL << CON_BBUSY)     	/*!< External interrupt line x */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uPMU_EXTIx_Init(uint8_t port, uint16_t bit, EXTI_Trigger_TypeDef triger);

FlagStatus EXTI_Line_Check(uint32_t EXTI_Line);

void EXTI_Clear_PendingBit(uint32_t EXTI_Line);
void EXTI_Set_PendingBit(uint32_t EXTI_Line);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_EXTI_H_ */

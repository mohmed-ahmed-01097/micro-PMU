/*******************************************************************************
 * @file    main.c
 * @author  MAAM
 * @email   mohmedahmed1097@gmail.com
 * @date    21.07.2021
 *          
 * @brief   main application called after startup
 * @note    
 *
*******************************************************************************/

/* Includes */
#include "systick.h"
#include "nvic.h"
#include "gpio.h"
#include "led.h"
#include "usart2.h"
#include "process.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** main function */
int main(void) {

#if (__FPU_USED == 1)
    SCB->CPACR |= (3UL << 20) | (3UL << 22);
   __DSB();
   __ISB();
#endif

	SysTick_Init();
	OnBoard_Init();

	uPMU_GPIO_Init();
	uPMU_NVIC_Init();
	uPMU_UART_Init();

	uPMU_Init_Process();

	/* Clear PRIMASK, enable IRQs */
	__enable_irq();
	USART2_Enable();

	/* Infinite loop */
	while(1) {

	    //USART2_Process();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

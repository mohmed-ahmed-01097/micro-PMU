/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Template/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "SysTick.h"
#include "process.h"
#include "exti.h"
#include "gpio.h"
#include "led.h"
#include "usart2.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void){

	SysTick_IncrementTicks_CallBack();

	/* Display Error Code */
	if(!(SysTick_GetCurrentTick() % SYSTICK_ERROR_CODE_DELAY)){
		//uPMU_Error_Code_Display();
	}
}

/* STM32F4xx Peripherals Interrupt Handlers */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set EXTI0 interrupt handlers */
void EXTI0_IRQHandler(void) {

	static uint8_t count = 0;

	if(EXTI_Line_Check(BUTTON_USER_Line)){

		/* Do your stuff when EXTI Line 0 */
		uPMU_Set_Error_Code(++count);

	    /* Clear interrupt flag */
		EXTI_Clear_PendingBit(BUTTON_USER_Line);
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set EXTI1 interrupt handlers */
void EXTI1_IRQHandler(void) {

	if(EXTI_Line_Check(uPMU_PROCESS_Line)){

		/* Do your stuff when EXTI Line 0 */
		uPMU_Str_Data();
		uPMU_Send_Data();

	    /* Clear interrupt flag */
		EXTI_Clear_PendingBit(uPMU_PROCESS_Line);
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set EXTI15_10 interrupt handlers */
void EXTI15_10_IRQHandler(void) {

 	if(EXTI_Line_Check(CON_ABUSY_Line) | EXTI_Line_Check(CON_BBUSY_Line)){

		/* Do your stuff when EXTI Line 0 */
		uPMU_AD7606_Reading();

	    /* Clear interrupt flag */
		EXTI_Clear_PendingBit(CON_ABUSY_Line);
		EXTI_Clear_PendingBit(CON_BBUSY_Line);

	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set DMA2_Stream0 interrupt handlers */
void DMA2_Stream0_IRQHandler(void){
	  /* Check transfer complete flag */
	  if(DMA_LISR_TCIF0 == (DMA_LISR_TCIF0 & DMA2->LISR))
	  {
	    /* DMA transfer is complete, turn off green LED */
			GPIO_Write_LED(LED3, Low);

	    /* Clear transfer complete flag */
	    DMA2->LIFCR = DMA_LIFCR_CTCIF0;
	  }
	  else
	  {
	    /* Turn on red LED, this interrupt is not handled */
			GPIO_Write_LED(LED4, High);
	  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set DMA1_Stream5 interrupt handlers */
void DMA1_Stream5_IRQHandler(void) {
	USART2_RX_DMA_IRQ_Callback();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set DMA1_Stream1 interrupt handlers */
void DMA1_Stream6_IRQHandler(void) {
	USART2_TX_DMA_IRQ_Callback();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Set USART2 interrupt handlers */
void USART2_IRQHandler(void) {
	USART2_IRQ_CallBack();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f40xx.s/startup_stm32f427x.s).                         */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

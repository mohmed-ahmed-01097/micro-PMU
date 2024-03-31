/*
 * usart2.h
 *
 *  Created on: Aug 4, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_USART2_H_
#define INC_USART2_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"


typedef enum {
  USART2_IDLE,
  USART2_WAIT_FOR_RESPONCE,
  USART2_ASK_FOR_NAME,
  USART2_WAIT_FOR_NAME,
  USART2_WAIT_FOR_COMMAND,
} USART_StateType;

typedef enum {
  USART2_NO_IRQ,
  USART2_CHAR_RECEIVED,
  USART2_PARITY_ERROR,
} USART_IRQStatusType;

/* PA2 -> USART2_TX, PA3 -> USART2_RX */
typedef enum {
	USART2_TX = 2, USART2_RX = 3,
} USART2_Typedef;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define USART_BAUD_RATE						115200

#define USARTDIV 							((float)(APB1_BUS_CLK / (16.0 * USART_BAUD_RATE)))

#define DIV_Mantissa 						((uint16_t)USARTDIV)
#define DIV_Fraction 						((uint8_t)((float)(USARTDIV - DIV_Mantissa) * 16.0 + 0.5))

#define MAX_BUFFER_LENGTH                   ((uint32_t) 20UL)

#define USART2_GPIO_PORT					GPIOA
#define USART2_GPIO_RCC_AHB1ENR				RCC_AHB1ENR_GPIOAEN

#define USART2_DMA							DMA1
#define USART2_DMA_CLOCK					RCC_AHB1ENR_DMA1EN
#define USART2_STREAM_CHSEL					DMA_SxCR_CHSEL_2

#define USART2_RX_STREAM					DMA1_Stream5
#define USART2_TX_STREAM					DMA1_Stream6
#define USART2_RX_DMA_TCIF					DMA_HISR_TCIF5
#define USART2_TX_DMA_TCIF					DMA_HISR_TCIF6
#define USART2_RX_DMA_FLAGS					(DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5)
#define USART2_TX_DMA_FLAGS					(DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6)

#define USART2_AF 							0x7		// AF7 USART2

static const char hello_world[]        = "Hello World!";
static const char ask_for_name[]       = "What is your name?";
static const char hi[]                 = "Hi,";
static const char ask_for_command[]    = "Please, send command";
static const char ask_for_command_ex[] = "Action[turn_on / turn_off] Led[green_led / red_led]";
static const char turn_on_green_led[]  = "turn_on green_led";
static const char turn_on_red_led[]    = "turn_on red_led";
static const char turn_off_green_led[] = "turn_off green_led";
static const char turn_off_red_led[]   = "turn_off red_led";
static const char done[]               = "Done";
static const char wrong_command[]      = "Wrong Command";
static const char parity_error[]       = "Parity Error";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uPMU_UART_Init(void);

void USART2_GPIO_Config(void);
void USART2_TX_DMA_Config(void);
void USART2_RX_DMA_Config(void);

void USART2_Init(void);
void USART2_Enable(void);

void USART2_IRQ_CallBack(void);
void USART2_TX_DMA_IRQ_Callback(void);
void USART2_RX_DMA_IRQ_Callback(void);

void USART2_Tx_Str(const char* str, uint32_t size);

void USART2_Process(void);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_USART2_H_ */

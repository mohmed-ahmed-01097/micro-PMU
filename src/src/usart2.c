/*
 * uart2.c
 *
 *  Created on: Aug 3, 2021
 *      Author: MAAM
 */

/* Includes */
#include "usart2.h"
#include <stddef.h>
#include "gpio.h"
#include "led.h"


static char RxChar = 0;
static uint8_t RxMessageLength = 0;
static char RxBuffer[MAX_BUFFER_LENGTH + 1];
static char RxDMABuffer[MAX_BUFFER_LENGTH];

static USART_StateType currentState = USART2_IDLE;
static USART_IRQStatusType currentIRQStatus = USART2_NO_IRQ;
static USART_IRQStatusType currentErrorStatus = USART2_NO_IRQ;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** UART Initialization function **/
void uPMU_UART_Init(void){

	USART2_GPIO_Config();

	USART2_Init();

	USART2_TX_DMA_Config();
	USART2_RX_DMA_Config();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Configure GPIO with AF7, USART2 connected to APB2 with 90MHz clock **/
void USART2_GPIO_Config(void){

	  /* Enable port A clock */
	  RCC->AHB1ENR |= USART2_GPIO_RCC_AHB1ENR;

	  uint16_t tx_bit = USART2_TX * 2;			/* wait to Enable*/
	  uint16_t rx_bit = USART2_RX * 2;

	  /* GPIO Mode as AF*/
	  USART2_GPIO_PORT->MODER   &= ~((Mode_AN	<<tx_bit) | (Mode_AN	<<rx_bit));
	  USART2_GPIO_PORT->MODER   |=   (Mode_AF	<<tx_bit) | (Mode_AF	<<rx_bit);

	  /* GPIO Output Type as Push Pull for Tx */
	  (PERIPH_BITBAND(((uint32_t)(&USART2_GPIO_PORT->OTYPER)), (tx_bit/2))) = OTYPER_PP;

	  /* GPIO AFR[] register */
	  USART2_GPIO_PORT->AFR[(tx_bit >> 4)] |= ((uint32_t)USART2_AF << (4 * ((tx_bit/2) & 0x7)));
	  USART2_GPIO_PORT->AFR[(rx_bit >> 4)] |= ((uint32_t)USART2_AF << (4 * ((rx_bit/2) & 0x7)));

	  /* GPIO Output Medium Speed */
	  USART2_GPIO_PORT->OSPEEDR &=~(OSPEEDR_VERY	<<tx_bit);
	  USART2_GPIO_PORT->OSPEEDR |= (OSPEEDR_MEDIUM	<<tx_bit);

	  /* GPIO no Pull */
	  USART2_GPIO_PORT->PUPDR   &= ~((PUPDR_RST	<<tx_bit) | (PUPDR_RST	<<rx_bit));
	  USART2_GPIO_PORT->PUPDR   |=   (PUPDR_NO	<<tx_bit) | (PUPDR_NO	<<rx_bit);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Configure DMA USART2 TX  **/
void USART2_TX_DMA_Config(void){

	/* Enable DMA clock in RCC */
	RCC->AHB1ENR |= USART2_DMA_CLOCK;

	/* Make sure that the DMA2 stream 7 is disabled */
	if(DMA_SxCR_EN == (DMA_SxCR_EN & USART2_TX_STREAM->CR)) {

	    /* DMA 2 stream 7 is enabled, shall be disabled first */
	    USART2_TX_STREAM->CR &= ~DMA_SxCR_EN;

	    /* Wait until EN bit is cleared */
	    while(DMA_SxCR_EN == (DMA_SxCR_EN & USART2_TX_STREAM->CR));
	}

	/* Select the DMA USART2 STREAM CHSEL in CHSEL[2:0] in the DMA_SxCR */
	USART2_TX_STREAM->CR &= ~DMA_SxCR_CHSEL;
	USART2_TX_STREAM->CR |= USART2_STREAM_CHSEL;

	/* Select stream priority very high */
	USART2_TX_STREAM->CR |= DMA_SxCR_PL;

	/* Select the data transfer direction memory-to-peripheral */
	USART2_TX_STREAM->CR &= ~DMA_SxCR_DIR;
	USART2_TX_STREAM->CR |=  DMA_SxCR_DIR_0;

	/* Select memory and peripherals sizes byte (8-bit) */
	USART2_TX_STREAM->CR &= ~DMA_SxCR_MSIZE;
	USART2_TX_STREAM->CR &= ~DMA_SxCR_PSIZE;

	/* Select FIFO mode */
	USART2_TX_STREAM->FCR |= DMA_SxFCR_DMDIS;

	/* Select FIFO threshold level half FIFO */
	USART2_TX_STREAM->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);

	/* Select memory incremented mode, peripheral shall has fixed address */
	USART2_TX_STREAM->CR |= DMA_SxCR_MINC;

	/* Enable DMA transfer complete interrupt */
	USART2_TX_STREAM->CR |= DMA_SxCR_TCIE;

	/* Set peripheral address */
	USART2_TX_STREAM->PAR = (uint32_t)&USART2->DR;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Configure DMA USART2 RX   **/
void USART2_RX_DMA_Config(void){

	/* Enable DMA clock in RCC */
	RCC->AHB1ENR |= USART2_DMA_CLOCK;

	/* Make sure that the DMA2 stream 5 is disabled */
	if(DMA_SxCR_EN == (DMA_SxCR_EN & USART2_RX_STREAM->CR)) {

		/* DMA USART2 TX STREAM is enabled, shall be disabled first */
		USART2_RX_STREAM->CR &= ~DMA_SxCR_EN;

		/* Wait until EN bit is cleared */
		while(DMA_SxCR_EN == (DMA_SxCR_EN & USART2_RX_STREAM->CR));
	}

	/* Select the DMA USART2 channel in CHSEL[2:0] in the DMA_SxCR */
	USART2_RX_STREAM->CR &= ~DMA_SxCR_CHSEL;
	USART2_RX_STREAM->CR |= USART2_STREAM_CHSEL;

	/* Select stream priority very high */
	USART2_RX_STREAM->CR |= DMA_SxCR_PL;

	/* Select the data transfer direction peripheral-to-memory */
	USART2_RX_STREAM->CR &= ~DMA_SxCR_DIR;

	/* Select memory and peripherals sizes byte (8-bit) */
	USART2_RX_STREAM->CR &= ~DMA_SxCR_MSIZE;
	USART2_RX_STREAM->CR &= ~DMA_SxCR_PSIZE;

	/* Select FIFO mode */
	USART2_RX_STREAM->FCR |= DMA_SxFCR_DMDIS;

	/* Select FIFO threshold level half FIFO */
	USART2_RX_STREAM->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);

	/* Select memory incremented mode, peripheral shall has fixed address */
	USART2_RX_STREAM->CR |= DMA_SxCR_MINC;

	/* Enable DMA transfer complete interrupt */
	USART2_RX_STREAM->CR |= DMA_SxCR_TCIE;

	/* Set peripheral address */
	USART2_RX_STREAM->PAR = (uint32_t)&USART2->DR;

	/* Set memory address */
	USART2_RX_STREAM->M0AR = (uint32_t)RxDMABuffer;

	/* Set number of data items */
	USART2_RX_STREAM->NDTR = MAX_BUFFER_LENGTH;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Configure USART2 for ST virtual COM port (VCP) **/
void USART2_Init(void){

	/* Configure USART2 */
	/* Enable USART2 clock */
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	/* Select OverSampling by 16 mode */
	USART2->CR1 &= ~USART_CR1_OVER8;
	/* Select 1 Start bit, 9 Data bits, n Stop bit */
	USART2->CR1 |=  USART_CR1_M;
	/* Select 1 stop bit */
	USART2->CR2 &= ~USART_CR2_STOP;
	/* Enable parity control */
	USART2->CR1 |= USART_CR1_PCE;
	/* Select odd parity */
	USART2->CR1 |= USART_CR1_PS;

	/* Select one sample bit method */
	USART2->CR3 |= USART_CR3_ONEBIT;

	/* Set baud rate = 115200 Bps
	 * USARTDIV = Fck / (16 * baud_rate)
	 *          = 90000000 / (16 * 115200) = 48.82
	 *
	 * DIV_Fraction = 16 * 0.82 = 13.12 = 13 = 0xD
	 * DIV_Mantissa = 48 = 0x30
	 *
	 * BRR          = 0x30D */

	USART2->BRR  = (DIV_Mantissa << 4)  + DIV_Fraction;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Enable USART2 transmitter and receiver **/
void USART2_Enable(void){

	/* Enable USART2 */
	USART2->CR1 |= USART_CR1_UE;

	/* Enable transmitter */
	USART2->CR1 |= USART_CR1_TE;
	/* Enable receiver */
	USART2->CR1 |= USART_CR1_RE;

	/* Enable reception buffer not empty flag interrupt */
	USART2->CR1 |= USART_CR1_RXNEIE;
	/* Enable parity error interrupt */
	USART2->CR1 |= USART_CR1_PEIE;
	/* Enable idle line detection interrupt */
	USART2->CR1 |= USART_CR1_IDLEIE;

	/* Enable DMA mode for transmitter and receiver */
	USART2->CR3 |= USART_CR3_DMAT;
	USART2->CR3 |= USART_CR3_DMAR;

    /* Clear all interrupt flags */
    USART2_DMA->HIFCR = USART2_RX_DMA_FLAGS;
    /* Enable DMA 2 stream 5 */
    USART2_RX_STREAM->CR |= DMA_SxCR_EN;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** IRQ callback function **/
void USART2_IRQ_CallBack(void){

	/* Check if parity error detected */
	if((USART2->SR & USART_SR_PE) == USART_SR_PE){
		/* Wait for RXNE flag to be set */
		while((USART2->SR & USART_SR_RXNE) != USART_SR_RXNE);

		/* Read data register to clear parity error */
		USART2->DR;

		/* Set parity error */
		currentIRQStatus = USART2_PARITY_ERROR;

	    /* Disable DMA stream for RX */
		USART2_RX_STREAM->CR &= ~DMA_SxCR_EN;
	}

	/* Check USART receiver */
	if((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE){

		/* Read character */
		RxChar = USART2->DR;

		/* Set IRQ status */
		currentIRQStatus = USART2_CHAR_RECEIVED;

	    /* Disable DMA stream for RX */
		USART2_RX_STREAM->CR &= ~DMA_SxCR_EN;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** IRQ callback function **/
void USART2_TX_DMA_IRQ_Callback(void){

	/* Check transfer complete flag */
	if(USART2_TX_DMA_TCIF == (USART2_TX_DMA_TCIF & USART2_DMA->HISR)) {

		/* Clear all interrupt flags */
		USART2_DMA->HIFCR = USART2_TX_DMA_FLAGS;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** IRQ callback function **/
void USART2_RX_DMA_IRQ_Callback(void){

	/* Check transfer complete flag */
	if(USART2_RX_DMA_TCIF == (USART2_RX_DMA_TCIF & USART2_DMA->HISR))
	  {
	    /* Calculate amount of data received */
	    RxMessageLength = MAX_BUFFER_LENGTH - USART2_RX_STREAM->NDTR;

	    /* Copy data into RX buffer */
	    for(int idx = 0; idx < RxMessageLength; idx++) {
	      RxBuffer[idx] = RxDMABuffer[idx];
	    }

	    /* Check error status */
	    if(currentErrorStatus != USART2_NO_IRQ) {

	    	/* Error detected, discard the received data */
	    	RxMessageLength = 0;
	    }

	    /* Clear all interrupt flags */
	    USART2_DMA->HIFCR = USART2_RX_DMA_FLAGS;

	    /* Enable DMA 2 stream 5 */
	    USART2_RX_STREAM->CR |= DMA_SxCR_EN;
	  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ErrorStatus strCmp(const char * str1, const char * str2, const uint8_t size) {

	/* Check null pointers */
	if((NULL != str1) && (NULL != str2)) {
		/* Start comparing */
		for (int idx = 0; idx < size; idx++) {
		  /* When not equal set the return status */
		  if(str1[idx] != str2[idx]) {
			  return ERROR;
		  }
		}
	  }
	  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** String Transmission **/
void USART2_Tx_Str(const char* str, uint32_t size){

	/* Check null pointers */
	if(str != NULL) {

	    /* Wait until DMA USART2 TX STREAM is disabled */
	    while(DMA_SxCR_EN == (DMA_SxCR_EN & USART2_TX_STREAM->CR));

	    /* Set memory address */
	    USART2_TX_STREAM->M0AR = (uint32_t)str;

	    /* Set number of data items */
	    USART2_TX_STREAM->NDTR = size;

	    /* Clear all interrupt flags */
	    USART2_DMA->HIFCR = USART2_TX_DMA_FLAGS;

	    /* Enable DMA1 stream 6 */
	    USART2_TX_STREAM->CR |= DMA_SxCR_EN;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** String Reception **/
static void USART2_Rx_Str(void){

	/* Local string buffer */
	static char RxLocalBuffer[MAX_BUFFER_LENGTH];

	/* Current reception index */
	static int RxIndex = 0;

	/* Check for end-of-line condition */
	if (RxChar == '\0') {

	    /* Check if string data was received before */
	    if (RxIndex != 0){

	    	/* Copy string data from local buffer */
	    	for(int idx = 0; idx < RxIndex; idx++) {
	    		RxBuffer[idx] = RxLocalBuffer[idx];
	    	}

	    	/* Add terminating NULL at the end */
	    	RxBuffer[RxIndex] = 0;

	    	/* Set message length */
	    	RxMessageLength = RxIndex + 1;

	    	/* Reset current index */
	    	RxIndex = 0;
	    }
	}else{

		/* Char data was received, Check for buffer overflow */
		if (MAX_BUFFER_LENGTH == RxIndex) {
			/* Reset the reception buffer */
			RxIndex = 0;
		}

		/* Copy char data into string buffer */
		RxLocalBuffer[RxIndex] = RxChar;

		/* Increment current index for the next char reception */
		RxIndex++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void USART2_Process(void) {

	/* Check error status */
	switch (currentIRQStatus)
	{
	case USART2_PARITY_ERROR:
		/* Transmit parity error */
		USART2_Tx_Str(parity_error, sizeof(parity_error));

		/* Reset USART2 state */
		currentState = USART2_IDLE;

		/* Reset IRQ status */
		currentIRQStatus = USART2_NO_IRQ;
		break;

	    case USART2_CHAR_RECEIVED:
	    	/* Receive string data */
	    	USART2_Rx_Str();

	    /* Reset IRQ status */
	    currentIRQStatus = USART2_NO_IRQ;
	    	break;

	    case USART2_NO_IRQ:
	    	break;

	    default:
	    	break;
	  }

	  /* Check current USART state */
	  switch (currentState)
	  {
	    case USART2_IDLE:
	      /* Transmit data */
	    	USART2_Tx_Str(hello_world, sizeof(hello_world));


	      /* Go to next state */
	      currentState = USART2_WAIT_FOR_RESPONCE;
	      break;

	    case USART2_WAIT_FOR_RESPONCE:
	      /* Check if new message received */
	      if(0 != RxMessageLength)
	      {
	        /* Reset message length */
	        RxMessageLength = 0;

	        /* Go to next state */
	        currentState = USART2_ASK_FOR_NAME;
	      }
	      else
	      {
	        /* Nothing received yet */
	      }
	      break;

	    case USART2_ASK_FOR_NAME:
	      /* Transmit data */
	    	USART2_Tx_Str(ask_for_name, sizeof(ask_for_name));

	      /* Go to next state */
	      currentState = USART2_WAIT_FOR_NAME;
	      break;

	    case USART2_WAIT_FOR_NAME:
	      /* Check if new message received */
	      if(0 != RxMessageLength)
	      {
	        /* Transmit data */
	    	  USART2_Tx_Str(hi, sizeof(hi));
	    	  USART2_Tx_Str(RxBuffer, RxMessageLength);
	    	  USART2_Tx_Str(ask_for_command, sizeof(ask_for_command));
	    	  USART2_Tx_Str(ask_for_command_ex, sizeof(ask_for_command_ex));


	        /* Reset message length */
	        RxMessageLength = 0;

	        /* Go to next state */
	        currentState = USART2_WAIT_FOR_COMMAND;
	      }
	      else
	      {
	        /* Nothing received yet */
	      }
	      break;

	    case USART2_WAIT_FOR_COMMAND:
	      /* Check if new message received */
	      if(0 != RxMessageLength)
	      {
	        /* Reset message length */
	        RxMessageLength = 0;

	        /* String compare results */
	        ErrorStatus isMatch_01 = ERROR;
	        ErrorStatus isMatch_02 = ERROR;
	        ErrorStatus isMatch_03 = ERROR;
	        ErrorStatus isMatch_04 = ERROR;

	        /* Compare with turn on green led command */
	        isMatch_01 =  strCmp(turn_on_green_led, RxBuffer,
	            sizeof(turn_on_green_led));

	        /* Check return status */
	        if(SUCCESS == isMatch_01)
	        {
	          /* Turn on green led */
	    		GPIO_Write_LED(LED3, High);

	          /* Transmit data */
	    		USART2_Tx_Str(done, sizeof(done));
	        }
	        else
	        {
	          /* Compare with turn on red led command */
	          isMatch_02 =  strCmp(turn_on_red_led, RxBuffer,
	              sizeof(turn_on_red_led));
	        }

	        /* Check return status */
	        if(SUCCESS == isMatch_02)
	        {
	          /* Turn on red led */
	          GPIO_Write_LED(LED4, High);

	          /* USART2_Tx_Str data */
	          USART2_Tx_Str(done, sizeof(done));

	        }
	        else if(ERROR == isMatch_01)
	        {
	          /* Compare with turn off green led command */
	          isMatch_03 =  strCmp(turn_off_green_led, RxBuffer,
	              sizeof(turn_off_green_led));
	        }
	        else
	        {
	          /* Do nothing */
	        }

	        /* Check return status */
	        if(SUCCESS == isMatch_03)
	        {
	          /* Turn off green led */
	            GPIO_Write_LED(LED3, Low);

	          /* Transmit data */
	            USART2_Tx_Str(done, sizeof(done));
	        }
	        else if((ERROR == isMatch_02)
	            && (ERROR == isMatch_01))
	        {
	          /* Compare with turn off red led command */
	          isMatch_04 =  strCmp(turn_off_red_led, RxBuffer,
	              sizeof(turn_off_red_led));
	        }
	        else
	        {
	          /* Do nothing */
	        }

	        /* Check return status */
	        if(SUCCESS == isMatch_04)
	        {
	          /* Turn off red led */
	          GPIO_Write_LED(LED4, Low);

	          /* Transmit data */
	          USART2_Tx_Str(done, sizeof(done));
	        }
	        else if((ERROR == isMatch_03)
	            && (ERROR == isMatch_02)
	            && (ERROR == isMatch_01))
	        {
	          /* Transmit data */
	        	USART2_Tx_Str(wrong_command, sizeof(wrong_command));
	        }
	        else
	        {
	          /* Do nothing */
	        }
	      }
	      else
	      {
	        /* Nothing received yet */
	      }
	      break;

	    default:
	      break;
	  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

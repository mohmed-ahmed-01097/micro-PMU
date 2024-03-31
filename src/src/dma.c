/*
 * dma.c
 *
 *  Created on: Aug 4, 2021
 *      Author: MAAM
 */

/* Includes */
#include "dma.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** DMA2 Stream0 initialization function **/
void DMA2_Stream0_Init(void){

	/* Enable DMA clock in RCC */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/* Make sure that the DMA2 stream 0 is disabled */
	if(DMA_SxCR_EN == (DMA_SxCR_EN & DMA2_Stream0->CR)) {

	    /* DMA 2 stream 0 is enabled, shall be disabled first */
	    DMA2_Stream0->CR &= ~DMA_SxCR_EN;

	    /* Wait until EN bit is cleared */
	    while(DMA_SxCR_EN == (DMA_SxCR_EN & DMA2_Stream0->CR));
	}

	/* Select the DMA channel CHSEL[2:0] in the DMA_SxCR.
	 * (not needed when M->M). */

	/* Select stream priority very high */
	DMA2_Stream0->CR |= DMA_SxCR_PL;

	/* Select the data transfer direction memory to memory */
	DMA2_Stream0->CR |= DMA_SxCR_DIR_1;

	/* Select memory and peripherals sizes 16 bit */
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

	/* Select burst transfers with 4 beats */
	DMA2_Stream0->CR |= DMA_SxCR_MBURST_0;
	DMA2_Stream0->CR |= DMA_SxCR_PBURST_0;

	/* Select FIFO mode, it will be also set by hardware when stream enabled */
	DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS;

	/* Select FIFO threshold level full FIFO */
	DMA2_Stream0->FCR |= DMA_SxFCR_FTH;

	/* Select FIFO threshold level half FIFO */
	//DMA2_Stream0->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);

	/* Select peripheral and memory incremented mode */
	DMA2_Stream0->CR |= DMA_SxCR_PINC;
	DMA2_Stream0->CR |= DMA_SxCR_MINC;

	/* Enable DMA transfer complete interrupt */
	DMA2_Stream0->CR |= DMA_SxCR_TCIE;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** DMA2 Stream0 enable function **/
void DMA2_Stream0_Enable(void){

	/* Clear all interrupt flags */
	DMA2->LIFCR = (DMA_LIFCR_CFEIF0 | DMA_LIFCR_CDMEIF0 | DMA_LIFCR_CTEIF0
				 | DMA_LIFCR_CHTIF0 | DMA_LIFCR_CTCIF0);

	/* Enable DMA 2 stream 0 */
	DMA2_Stream0->CR |= DMA_SxCR_EN;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** DMA2 Stream0 set addresses function **/
void DMA2_Stream0_Set_Addresses(const uint32_t * src, uint32_t * dst, const uint32_t size){

	/* Set the source address to the peripheral port */
	DMA2_Stream0->PAR = (uint32_t)src;

	/* Set the destination address to the memory port */
	DMA2_Stream0->M0AR = (uint32_t)dst;

	/* Set number of data items */
	DMA2_Stream0->NDTR = size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///** DMA1 Stream5 initialization function **/
//void DMA1_Stream5_Init(void){
//
//	/* Enable DMA clock in RCC */
//	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
//
//	/* Make sure that the DMA1 stream 5 is disabled */
//	if(DMA_SxCR_EN == (DMA_SxCR_EN & DMA1_Stream5->CR)) {
//
//	    /* DMA1 stream5 is enabled, shall be disabled first */
//	    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
//
//	    /* Wait until EN bit is cleared */
//	    while(DMA_SxCR_EN == (DMA_SxCR_EN & DMA1_Stream5->CR));
//	}
//
//	/* Select the DMA channel CHSEL[2:0] in the DMA_SxCR.
//	 * (not needed when M->M). */
//
//	/* Select stream priority very high */
//	DMA1_Stream5->CR |= DMA_SxCR_PL;
//
//	/* Select the data transfer direction memory to memory */
//	DMA1_Stream5->CR |= DMA_SxCR_DIR_1;
//
//	/* Select memory and peripherals sizes */
//	DMA1_Stream5->CR |= DMA_SxCR_MSIZE_1;
//	DMA1_Stream5->CR |= DMA_SxCR_PSIZE_1;
//
//	/* Select burst transfers with 4 beats */
//	DMA1_Stream5->CR |= DMA_SxCR_MBURST_0;
//	DMA1_Stream5->CR |= DMA_SxCR_PBURST_0;
//
//	/* Select FIFO mode, it will be also set by hardware when stream enabled */
//	DMA1_Stream5->FCR |= DMA_SxFCR_DMDIS;
//
//	/* Select FIFO threshold level half FIFO */
//	DMA1_Stream5->FCR |= (DMA_SxFCR_FTH_0 | DMA_SxFCR_FTH_1);
//
//	/* Select peripheral and memory incremented mode */
//	DMA1_Stream5->CR |= DMA_SxCR_PINC;
//	DMA1_Stream5->CR |= DMA_SxCR_MINC;
//
//	/* Enable DMA transfer complete interrupt */
//	DMA1_Stream5->CR |= DMA_SxCR_TCIE;
//
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///** DMA1 Stream5 enable function **/
//void DMA1_Stream5_Enable(void){
//
//	/* Clear all interrupt flags */
//	DMA1->HIFCR = (DMA_HIFCR_CFEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CTEIF5
//				 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTCIF5);
//
//	/* Enable DMA1 stream5 */
//	DMA1_Stream5->CR |= DMA_SxCR_EN;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///** DMA1 Stream5 set addresses function **/
//void DMA1_Stream5_Set_Addresses(const uint32_t * src, uint32_t * dst, const uint32_t size){
//
//	/* Set the source address to the peripheral port */
//	DMA1_Stream5->PAR = (uint32_t)src;
//
//	/* Set the destination address to the memory port */
//	DMA1_Stream5->M0AR = (uint32_t)dst;
//
//	/* Set number of data items */
//	DMA1_Stream5->NDTR = size;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

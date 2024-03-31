/*
 * process.c
 *
 *  Created on: Aug 7, 2021
 *      Author: MAAM
 */

/* Includes */
#include "process.h"
#include "usart2.h"
#include "gpio.h"
#include "exti.h"
#include "led.h"

char DATA_String[(ADC_CHANNELn * ADCn + 1) * 14 + 3];
Channel_Typedef ADC_BUFFER[ADC_CHANNELn];
OUTPUT_Typedef  OUT_PHASOR[ADC_CHANNELn];
Result_Typedef  OUT_F;

static volatile Channel_Typedef ADC_ARRAY[ADCn][READ_SAMPLEn][ADC_CHANNELn];

volatile uint16_t SAMPLE = 0;
volatile uint8_t Read_Buffer = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Initialize uPMU Process **/
void uPMU_Init_Process(void){

	LookUp_Init();
//	for(uint16_t i = READ_SAMPLEn; i-- ; ){
//		for(uint8_t j = ADC_CHANNELn; j-- ; ){
//			ADC_ARRAY[0][i][j].V = 0;
//			ADC_ARRAY[0][i][j].A = 0;
//			ADC_ARRAY[1][i][j].V = 0;
//			ADC_ARRAY[1][i][j].A = 0;
//		}
//	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** uPMU ADC AD7606 Reading **/
void uPMU_AD7606_Reading(void) {

	//uint32_t time = SysTick->VAL;
	GPIO_Write_LED(LED4, High);

	volatile Channel_Typedef* Read = ADC_ARRAY[Read_Buffer    ][SAMPLE];
	volatile Channel_Typedef* read = ADC_ARRAY[Read_Buffer ^ 1][SAMPLE];
	volatile Channel_Typedef* old  = ADC_BUFFER; //ADC_ARRAY[READ_SAMPLEn];


	for(uint8_t i = ADC_CHANNELn; i-- ; Read++, read++, old++){

		/* Start reading falling edge */
		ODR_CON_CS_BITBAND = Low;

		/* Select A_ADC */
		ODR_CON_SE_BITBAND = Low;

		/* Read Port 16bit A_ADC Data */
		old ->V = read->V;
		Read->V = DATA_GPIO_PORT->IDR;

		/* Select B_ADC */
		ODR_CON_SE_BITBAND = High;

		/* Read Port 16bit B_ADC Data */
		old ->A = read->A;
		Read->A = DATA_GPIO_PORT->IDR;

		/* End reading rising edge */
		ODR_CON_CS_BITBAND = High;


//		if((i == 0) && !(IDR_CON_AFRST_BITBAND && IDR_CON_BFRST_BITBAND)) {
//
//			/* Reset reading */
//			/* Start Reset Pulse rising edge */
//			ODR_CON_RES = High;
//			/* End Reset Pulse falling edge */
//			ODR_CON_RES = Low;
//			break;
//		}

	}

	uPMU_Process_Mode(ADC_ARRAY[Read_Buffer][SAMPLE], SAMPLE);

	/* Index Increment */
	if((++SAMPLE) >= READ_SAMPLEn){
		SAMPLE = 0;
		Read_Buffer ^= 1;
		EXTI_Set_PendingBit(uPMU_PROCESS_Line);
	}

	GPIO_Write_LED(LED4, Low);
	//time -= SysTick->VAL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** uPMU Data Process **/
void uPMU_Str_Data(void) {

	//uint32_t time1 = SysTick->VAL;

	//GPIO_Write_LED(LED4, High);
	uPMU_Get_Result();
	//printf("%i\t%i\t%i\r\n",(int)((OUT_PHASOR[0].V.Absl + 0.0005) * 1000),(int)((OUT_PHASOR[0].V.Angl + 0.005) * 100),(int)(OUT_PHASOR[0].F.Absl*100));

	char* str = DATA_String;


	for(uint8_t i = 0 ; i < ADC_CHANNELn ; i++){
		//*str++ = 'V';
		//*str++ = i + '0';
		//*str++ = '=';
		uPMU_Str_Result(&OUT_PHASOR[i].V, &str);
	}
	//*str++ = '\n';

	for(uint8_t i = 0 ; i < ADC_CHANNELn ; i++){
		//*str++ = 'A';
		//*str++ = i + '0';
		//*str++ = '=';
		uPMU_Str_Result(&OUT_PHASOR[i].A, &str);
	}
	//*str++ = '\n';

	//*str++ = 'F';
	//*str++ = '=';
	uPMU_Str_Freq(&OUT_F, &str);

	*str++ = '\n';
	*str++ = '\r';
	*str++ = '\0';

	//uPMU_Send_Data();

	//GPIO_Write_LED(LED4, Low);
	//time1 -= SysTick->VAL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** uPMU Data Process **/
void uPMU_Str_Result(const Result_Typedef* out, char** str) {

	uint32_t data = (uint32_t)((out->Absl + 0.0005) * 1000.0);

	*((*str)++) = (data / 100000) % 10 + '0';
	*((*str)++) = (data / 10000 ) % 10 + '0';
	*((*str)++) = (data / 1000  ) % 10 + '0';
	*((*str)++) = '.';
	*((*str)++) = (data / 100   ) % 10 + '0';
	*((*str)++) = (data / 10    ) % 10 + '0';
	*((*str)++) = (data         ) % 10 + '0';
	//*((*str)++) = '<';

	data = (uint32_t)((out->Angl + 0.0005 ) * 1000.0 );

	*((*str)++) = (data / 100000) % 10 + '0';
	*((*str)++) = (data / 10000 ) % 10 + '0';
	*((*str)++) = (data / 1000  ) % 10 + '0';
	*((*str)++) = '.';
	*((*str)++) = (data / 100   ) % 10 + '0';
	*((*str)++) = (data / 10    ) % 10 + '0';
	*((*str)++) = (data         ) % 10 + '0';
	//*((*str)++) = '#';

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** uPMU Data Process **/
void uPMU_Str_Freq(const Result_Typedef* out, char** str) {

	uint32_t data = (uint32_t)((out->Absl + 0.00005) * 10000.0);

	*((*str)++) = (data / 100000) % 10 + '0';
	*((*str)++) = (data / 10000 ) % 10 + '0';
	*((*str)++) = '.';
	*((*str)++) = (data / 1000  ) % 10 + '0';
	*((*str)++) = (data / 100   ) % 10 + '0';
	*((*str)++) = (data / 10    ) % 10 + '0';
	*((*str)++) = (data         ) % 10 + '0';
	//*((*str)++) = '<';

	data = (uint32_t)((out->Angl + 0.00005) * 10000.0);

	*((*str)++) = (data / 100000) % 10 + '0';
	*((*str)++) = (data / 10000 ) % 10 + '0';
	*((*str)++) = '.';
	*((*str)++) = (data / 1000  ) % 10 + '0';
	*((*str)++) = (data / 100   ) % 10 + '0';
	*((*str)++) = (data / 10    ) % 10 + '0';
	*((*str)++) = (data         ) % 10 + '0';
	//*((*str)++) = '#';

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void uPMU_Send_Data(void){
	USART2_Tx_Str(DATA_String,sizeof(DATA_String));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

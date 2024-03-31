/*
 * estimation.h
 *
 *  Created on: Aug 23, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_ESTIMATION_H_
#define INC_ESTIMATION_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	float Real;
	float Imag;
}Complex_Typedef;

typedef struct{
	float Absl;
	float Angl;
}Result_Typedef;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	int16_t V;
	int16_t A;
}Channel_Typedef;

typedef struct{
	Complex_Typedef V;
	Complex_Typedef A;
}DFT_Typedef;

typedef struct{
	Result_Typedef V;
	Result_Typedef A;
}Phasor_Typedef;

typedef struct{
	Result_Typedef V;
	Result_Typedef A;
}OUTPUT_Typedef;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LookUp_Init(void);
void uPMU_Get_Result(void);

void uPMU_Process_Mode(volatile Channel_Typedef* data, const uint16_t sample);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_ESTIMATION_H_ */

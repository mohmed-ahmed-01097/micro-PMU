/*
 * process.h
 *
 *  Created on: Aug 7, 2021
 *      Author: MAAM
 */

/* Define to prevent recursive inclusion */
#ifndef INC_PROCESS_H_
#define INC_PROCESS_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"
#include "config.h"
#include "estimation.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void uPMU_Init_Process(void);

void uPMU_AD7606_Reading(void);

void uPMU_Str_Data(void);
void uPMU_Str_Result(const Result_Typedef* out, char** str);
void uPMU_Str_Freq  (const Result_Typedef* out, char** str);
void uPMU_Send_Data(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_PROCESS_H_ */

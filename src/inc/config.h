/*******************************************************************************
 * @file    config.h
 * @author  MAAM
 * @email   mohmedahmed1097@gmail.com
 * @date    21.07.2021
 *
 * @brief   Project Configuration
 * @note
 *
*******************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32f4xx.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CPU_PLL_CLK             	168000000UL         // 168 MHz
#define AHB_BUS_CLK             	168000000UL         // 168 MHz
#define APB1_BUS_CLK            	 42000000UL         // 42  MHz
#define APB2_BUS_CLK            	 84000000UL         // 84  MHz

/* ADC Modules Constant */
#define ADCn							2U
#define ADC_CHANNELn					5U				//8U
#define READ_SAMPLEn					512U
#define PHASEn							3U

#define FREQUENCY 						50U

#define PI								3.14159265358979f
#define ADC_RESOLUTION					32767.0f
#define V_RATIO							5.0f
#define A_RATIO							5.0f

/* Error Code Number */
#define ERRORn                      	4U

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Bit Band for Peripherals **/
#define PERIPH_BITBAND(REG_ADD, BIT) 	*((volatile uint32_t *)((PERIPH_BB_BASE + ((REG_ADD - PERIPH_BASE) * 32) + (BIT * 4))))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_CONFIG_H_ */

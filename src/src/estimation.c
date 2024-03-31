/*
 * estimation.c
 *
 *  Created on: Aug 23, 2021
 *      Author: MAAM
 */

/* Includes */
#include <math.h>
#include "estimation.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TAYLOR_LENGH			10U
#define TAN_1					1.557407724654902f

const float TAN_F[TAYLOR_LENGH] = {1.0f/3, 1.0f/5, 1.0f/7, 1.0f/9, 1.0f/11, 1.0f/13, 1.0f/15, 1.0f/17, 1.0f/19, 1.0f/21};

const float PI2    = (PI * 2);
const float PI_2   = (PI / 2);
const float PI_4   = (PI / 4);

const float Delta_t = 1.0f / (READ_SAMPLEn * FREQUENCY);
const float Freq_t  = 1.0f / (READ_SAMPLEn * FREQUENCY) * ((READ_SAMPLEn - 1) / READ_SAMPLEn);

const float DEGREE = (180.0 / PI);
const float FACTOR = (2 * 22.24f / (READ_SAMPLEn * READ_SAMPLEn * ADC_RESOLUTION * ADC_RESOLUTION));

static float mat_i[3*3];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern Result_Typedef  OUT_F;
extern Channel_Typedef ADC_BUFFER[ADC_CHANNELn];
extern OUTPUT_Typedef  OUT_PHASOR[ADC_CHANNELn];
extern volatile uint8_t Read_Buffer;

static float SIN_ARRAY[READ_SAMPLEn];
static float COS_ARRAY[READ_SAMPLEn];

static Result_Typedef SUM_F;
static DFT_Typedef    DFT_PHASOR[ADC_CHANNELn];
static OUTPUT_Typedef SUM_PHASOR[ADC_CHANNELn];
static Phasor_Typedef OUT_ARRAY[ADCn][READ_SAMPLEn][ADC_CHANNELn];


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** generate sin **/
static void Set_sin(float* float_ptr){

	for(uint16_t i = READ_SAMPLEn ; i-- ; --float_ptr){
		*float_ptr = sin(2*PI*i / READ_SAMPLEn);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** generate cos **/
static void Set_cos(float* float_ptr){

	for(uint16_t i = READ_SAMPLEn ; i-- ; --float_ptr){
		*float_ptr = cos(2*PI*i / READ_SAMPLEn);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Adjust Angle */
static float  Adjust_Angle(float angle_f){

	int angle_i = (int)(angle_f/360);

	angle_f -= (float)(angle_i * 360);

	if(angle_f < 0){
		angle_f += 360;
	}
	return angle_f;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** generate arctan **/
static float Get_atan(float y, float x){

	float angle;

	if (x == 0){
		angle = PI_2 * (y != 0);
	}else{
		uint8_t flag = 0;
		float taylor = y / x;

		if(taylor < 0){
			taylor *= -1;
		}
		if(taylor > 1){
			taylor = 1 / taylor;
			flag = 1;
		}
		if(taylor > 0.5){
			taylor = (TAN_1 - taylor) / (1 + TAN_1 * taylor);
			flag = flag + 2;
		}

		float temp = taylor;
		float step = -taylor * taylor;
		angle = temp;
		for(uint8_t i = 0; i < TAYLOR_LENGH ; i++){
			temp  = temp * step * TAN_F[i];
			angle = angle + temp;
		}
		if(flag >= 2){
			angle = 1 - angle;
			flag = flag - 2;
		}
		if(flag == 1){
			angle = PI_2 - angle;
		}
	}

	if(x < 0){
		if(y < 0){
			return (angle - PI);
		}else{
			return (PI - angle);
		}
	}else if(y < 0){
		return (-angle);
	}

	return (angle);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** generate J Matrix **/
static void Set_MAT_J(void){

	float sum_b1, sum_b2, sum_b3, sum_b4;
	sum_b1 = sum_b2 = sum_b3 = sum_b4 = 0;

	for(uint16_t i = READ_SAMPLEn; i-- ; ){

		float b = i * Delta_t;
		float step_b = b;

		sum_b1 += step_b ;		step_b = step_b * b;
		sum_b2 += step_b; 		step_b = step_b * b;
		sum_b3 += step_b; 		step_b = step_b * b;
		sum_b4 += step_b;
	}
	const float mat_m[3][3]  = {
		{READ_SAMPLEn, sum_b1, sum_b2},
		{sum_b1,       sum_b2, sum_b3},
		{sum_b2,       sum_b3, sum_b4},
	};

	float determinant = 0;
	for(uint8_t i = 3; i-- ; ){
		determinant += (mat_m[0][i]*(mat_m[1][(i+1)%3]*mat_m[2][(i+2)%3] - mat_m[1][(i+2)%3]*mat_m[2][(i+1)%3]));
	}
	for(uint8_t i = 3; i-- ; ){
		for(uint8_t j = 3 ; j-- ; ){
	        	mat_i[i*3+j] = ((mat_m[(i+1)%3][(j+1)%3] * mat_m[(i+2)%3][(j+2)%3]) - (mat_m[(i+1)%3][(j+2)%3]*mat_m[(i+2)%3][(j+1)%3]))/ determinant;
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Initialization of Look Up tables **/
void LookUp_Init(void){

	/* Init values if Sin and Cos */
	Set_sin(&SIN_ARRAY[READ_SAMPLEn-1]);
	Set_cos(&COS_ARRAY[READ_SAMPLEn-1]);

	Set_MAT_J();

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Read Process **/
void uPMU_Process_Mode(volatile Channel_Typedef* data, const uint16_t sample){

	//uint32_t time = SysTick->VAL;

	float sin_f = SIN_ARRAY[sample];
	float cos_f = COS_ARRAY[sample];

	DFT_Typedef*    value   = DFT_PHASOR;
	OUTPUT_Typedef* out_sum = SUM_PHASOR;
	Phasor_Typedef* result  = OUT_ARRAY[Read_Buffer][sample];

	Phasor_Typedef*  result_0 = result;
	Channel_Typedef* data_0   = ADC_BUFFER;


	for(uint8_t i = ADC_CHANNELn; i-- ;  ++value, ++data, ++data_0, ++result, ++out_sum){

		value->V.Real += ((data->V - data_0->V) * cos_f);
		value->A.Real += ((data->A - data_0->A) * cos_f);

		value->V.Imag += ((data->V - data_0->V) * sin_f);
		value->A.Imag += ((data->A - data_0->A) * sin_f);

		result->V.Absl = (value->V.Real * value->V.Real + value->V.Imag * value->V.Imag) * FACTOR;
		result->A.Absl = (value->A.Real * value->A.Real + value->A.Imag * value->A.Imag) * FACTOR;

		result->V.Angl = Get_atan(-value->V.Imag, value->V.Real);
		result->A.Angl = Get_atan(-value->A.Imag, value->A.Real);

		out_sum->V.Absl += result->V.Absl;
		out_sum->A.Absl += result->A.Absl;

		out_sum->V.Angl += Adjust_Angle(result->V.Angl  * DEGREE);
		out_sum->A.Angl += Adjust_Angle(result->A.Angl  * DEGREE);

//		result->V.Angl = Adjust_Angle(Get_atan(-value->V.Imag, value->V.Real) * DEGREE);
//		result->A.Angl = Adjust_Angle(Get_atan(-value->A.Imag, value->A.Real) * DEGREE);
//
//		out_sum->V.Angl += result->V.Angl;
//		out_sum->A.Angl += result->A.Angl;

	}

    float b_1 = sample * Delta_t;
    float b_2 = b_1 * b_1;
    //float J0 = mat_i[0] + mat_i[1] * b_1 + mat_i[2] * b_2;
    float J1 = mat_i[3] + mat_i[4] * b_1 + mat_i[5] * b_2;
    float J2 = mat_i[6] + mat_i[7] * b_1 + mat_i[8] * b_2;

    SUM_F.Absl += (result_0->V.Angl * J1);
    SUM_F.Angl += (result_0->V.Angl * J2);

//	float angle = (result_0->V.Angl - 180.0) / DEGREE;
//	SUM_F.Absl += (angle * J1);
//	SUM_F.Angl += (angle * J2);


	//time -= SysTick->VAL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** Get Result **/
void uPMU_Get_Result(void){

	OUTPUT_Typedef* out_sum = SUM_PHASOR;
	OUTPUT_Typedef* out     = OUT_PHASOR;

	for(uint8_t i = ADC_CHANNELn; i-- ; ++out, ++out_sum){

		out->V.Absl = out_sum->V.Absl / READ_SAMPLEn;
		out->A.Absl = out_sum->A.Absl / READ_SAMPLEn;

		out->V.Angl = out_sum->V.Angl / READ_SAMPLEn;
		out->A.Angl = out_sum->A.Angl / READ_SAMPLEn;

		*(out_sum) = (OUTPUT_Typedef){{0.0, 0.0}, {0.0, 0.0}};

	}

	OUT_F.Angl  = SUM_F.Angl / PI;
	OUT_F.Absl  = FREQUENCY + SUM_F.Absl / PI2 + OUT_F.Angl * Freq_t;

	SUM_F.Absl = 0;
	SUM_F.Angl = 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

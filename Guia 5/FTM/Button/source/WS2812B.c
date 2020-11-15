/*
 * WS2812B.c
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#include "header/WS2812B.h"
#include "header/DMA.h"
#include "header/FTM.h"
#include "header/PORT.h"

#define CNV_ON 39 //39 ticks -> 0.8us
#define CNV_OFF 22 //22 ticks -> 0.46us

#define MAT_SIZE 64*3*2
#define ROW_SIZE 8

typedef struct{
	uint16_t G;
	uint16_t R;
	uint16_t B;
}GRB_t;
static int8_t ftmid1;
static GRB_t led_matrix [64]__attribute__((aligned(512)));

void WS2812B_init(void){
	uint8_t i;
	uint8_t j;

	for(i = 0; i < 64; i++){
		led_matrix[i].G = CNV_OFF;
		led_matrix[i].R = CNV_OFF;
		led_matrix[i].B = CNV_OFF;
	}

	led_matrix[1].G = CNV_ON;
	led_matrix[1].R = CNV_ON;
	led_matrix[1].B = CNV_ON;

	DMAInitWS2812b((uint16_t*)(&led_matrix), MAT_SIZE);

	FTM_DATA data;
	PORT_Init();
	data.CNTIN = 0;
	data.MODULO = 0x0062;	//62 ticks -> 1.26us
	data.CNV = 0x005;
	data.EPWM_LOGIC = FTM_lAssertedHigh;
	data.MODE = FTM_mPulseWidthModulation;
	data.PSC = FTM_PSC_x1;
	data.DMA = 1;
	ftmid1 = FTMInit(2, 1, data); //FTM0 CH0
	FTMStartClock(0);
}

void WS2812B_matrix_set(uint8_t color, uint8_t row, uint8_t col, uint8_t mode){

	switch(color){
	case 0:
		if(mode)
			led_matrix[ROW_SIZE*row+col].G = CNV_ON;
		else
			led_matrix[ROW_SIZE*row+col].G = CNV_OFF;
		break;
	case 1:
		if(mode)
			led_matrix[ROW_SIZE*row+col].R = CNV_ON;
		else
			led_matrix[ROW_SIZE*row+col].R = CNV_OFF;
		break;
	case 2:
		if(mode)
			led_matrix[ROW_SIZE*row+col].B = CNV_ON;
		else
			led_matrix[ROW_SIZE*row+col].B = CNV_OFF;
		break;
	default: break;
	}

}






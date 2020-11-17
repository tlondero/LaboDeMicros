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
#define CNV_ZERO 2
#define MOD 63//62ticks ->1.26us

#define MAT_SIZE (64+2)*8*3*2 //64 LEDS * 8BITS * 3 COLORES * 2bytes (CNV son 16 bits)
#define ROW_SIZE 8

typedef enum {RED, GREEN, BLUE} led_color;

typedef struct{
	uint16_t G[8]; //Un array de 8 elementos de 16 bits.
	uint16_t R[8];
	uint16_t B[8];
}GRB_t;
static int8_t ftmid1;
static GRB_t led_matrix [64+2];//__attribute__((aligned(4096)));

static void set_led_brightness(uint16_t *ptr, uint8_t brightness){
	uint8_t i;
	for (i = 0; i<8; i++){
		ptr[i] = (brightness & (1<<i)) ? CNV_ON : CNV_OFF;
	}

}
void WS2812B_init(void){
	uint8_t i;
	uint8_t j;

	for(i = 0; i < 64; i++){

		set_led_brightness(led_matrix[i].R, 255);
		set_led_brightness(led_matrix[i].G, 255);
		set_led_brightness(led_matrix[i].B, 255);

	}
	//Add pause leds
	for(i = 0; i < 8; i++){

		led_matrix[64].R[i] = CNV_ZERO;
		led_matrix[64].G[i] = CNV_ZERO;
		led_matrix[64].B[i] = CNV_ZERO;

		led_matrix[65].R[i] = CNV_ZERO;
		led_matrix[65].G[i] = CNV_ZERO;
		led_matrix[65].B[i] = CNV_ZERO;
	}




/*
	set_led_brightness(led_matrix[0].R, 255);
	set_led_brightness(led_matrix[0].G, 255);
	set_led_brightness(led_matrix[0].B, 255);
*/
	DMAInitWS2812b((uint16_t*)(&led_matrix), MAT_SIZE);

	FTM_DATA data;
	PORT_Init();
	data.CNTIN = 0;
	data.MODULO = MOD;	//62 ticks -> 1.26us
	data.CNV = 0x005;
	data.EPWM_LOGIC = FTM_lAssertedHigh;
	data.MODE = FTM_mPulseWidthModulation;
	data.PSC = FTM_PSC_x1;
	data.DMA = 1;
	ftmid1 = FTMInit(2, 1, data); //FTM0 CH0
	FTMStartClock(0);
}



void WS2812B_matrix_set(uint8_t color, uint8_t brightness, uint8_t row, uint8_t col){

	switch(color){
	case GREEN:
		set_led_brightness(led_matrix[ROW_SIZE*row+col].G, brightness);
		break;
	case RED:
		set_led_brightness(led_matrix[ROW_SIZE*row+col].R, brightness);
		break;
	case BLUE:
		set_led_brightness(led_matrix[ROW_SIZE*row+col].B, brightness);
		break;
	default: break;
	}

}






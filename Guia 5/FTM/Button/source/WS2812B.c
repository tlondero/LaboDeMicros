/*
 * WS2812B.c
 *
 *  Created on: Nov 14, 2020
 *      Author: Acer
 */

#include "header/WS2812B.h"
#include "header/DMA.h"
#include "header/FTM.h"
#include "header/timer.h"
#include "header/PORT.h"

#define CNV_ON 39 //39 ticks -> 0.8us
#define CNV_OFF 20 //20 ticks -> 0.4us
#define CNV_ZERO 0
#define MOD 62//62+1ticks ->1.26us

#define CANT_LEDS 64
#define CANT_LEDS_ZERO 0

#define MAT_SIZE ((CANT_LEDS+CANT_LEDS_ZERO)*8*3*2)+(1*2) //(64 LEDS+10LEDS en zero para reset) * 8BITS * 3 COLORES * 2bytes (CNV son 16 bits)
#define ROW_SIZE 8

typedef enum {RED, GREEN, BLUE} led_color;

typedef struct{
	uint16_t G[8]; //Un array de 8 elementos de 16 bits.
	uint16_t R[8];
	uint16_t B[8];
}GRB_t;
static int8_t ftmid1;
static int8_t timerid;
static GRB_t led_matrix [CANT_LEDS+CANT_LEDS_ZERO];

static void tim_cb(void){
	if(led_matrix[0].G[0] == CNV_ON){
		FTMSetCnV(ftmid1, CNV_ON);
	}
	else{
		FTMSetCnV(ftmid1, CNV_OFF);
	}
	FTMStartClock(0);
}

static void dma_cb(void){
	FTMStopClock(0);
	timerReset(timerid);
}

static void set_color_brightness(uint16_t *ptr, uint8_t brightness){
	uint8_t i;
	for (i = 0; i<8; i++){
		ptr[i] = (brightness & (1<<i)) ? CNV_ON : CNV_OFF;
	}

}
void WS2812B_init(void){
	uint8_t i;

	for(i = 0; i < CANT_LEDS; i++){

		set_color_brightness(led_matrix[i].R, 0);
		set_color_brightness(led_matrix[i].G, 0);
		set_color_brightness(led_matrix[i].B, 0);

	}

	timerInit();
	timerid = timerGetId();
	timerStart(timerid, 2, TIM_MODE_SINGLESHOT, tim_cb);
	timerStop(timerid);

	DMAInitWS2812b((uint16_t*)(&led_matrix), MAT_SIZE, dma_cb);

	FTM_DATA data;
	PORT_Init();
	data.CNTIN = 0;
	data.MODULO = MOD;	//62 ticks -> 1.26us
	data.CNV = 22;
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





